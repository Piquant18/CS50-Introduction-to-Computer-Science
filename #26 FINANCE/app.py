import os
from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

app = Flask(__name__)

app.jinja_env.globals['usd'] = usd              # <td>{{ stock.price | usd }}</td> / Si stock.price = 1000, esto lo convertiría en "$1,000.00".

app.config["SESSION_PERMANENT"] = False             #Indica que la sesión no será permanente (se borra al cerrar el navegador)
app.config["SESSION_TYPE"] = "filesystem"              #A lmacena la sesión en archivos locales (en el proyecto) en lugar de usar cookies.
Session(app)                                        # Aplica la configuración de sesiones al proyecto.

db = SQL("sqlite:///finance.db")


@app.after_request                  # Indica que esta función se ejecutará después de cada solicitud al servidor.
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0                                                     # evita todo tipo de cache asi tenga que reiniciar la pagina
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")         #todo esto se ejecuta cuando este en la pagina principal osea index
@login_required         #necesita estar autentificado
def index():
    """Show portfolio of stocks"""

    user_id = session["user_id"]

    user_cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]

    #consulta en la tabla transactions las acciones symbol compradas / suma todas las acciones de un mismo tipo / agrupa por simbolo de accion / se asegura de que las acciones sean positivos
    stocks = db.execute("""
        SELECT symbol, SUM(shares) AS shares
        FROM transactions
        WHERE user_id = ?
        GROUP BY symbol
        HAVING SUM(shares) > 0
    """, user_id)

    portfolio = []      #crea lista vacia
    total_value = 0     #variable para acumular todas las acciones

    for stock in stocks:
        symbol = stock["symbol"]            # toma symbol y shares de el anterior de la base osea = AAPL - 5
        shares = stock["shares"]

        stock_data = lookup(symbol)         # lookup(symbol): Llama a la función lookup() (de helpers.py) para obtener el precio actual de la acción segun su nombre.-----------------------------------------------------------------------------
        price = stock_data["price"]         # Extrae el precio del diccionario retornado.
        total = shares * price              # el precio de la accion que cosulta por el shares de arriba el 5 xd
        total_value += total                # el resultado lo suma a la variable de arriba

        portfolio.append({              # Crea un diccionario con los datos de la acción y lo añade a portfolio.
            "symbol": symbol, "shares": shares, "price": price, "total": total
        })

    grand_total = total_value + user_cash       #Calcular el valor total de la cuenta total value y el efectivo de user cash

    return render_template("index.html", portfolio=portfolio, cash=user_cash, grand_total=grand_total)      # Renderiza index.html y le pasa: / portfolio: Lista con las acciones del usuario / cash: Dinero en efectivo del usuario / grand_total: Valor total de la cuenta.----------------------------------------------------------------------


@app.route("/buy", methods=["GET", "POST"])     #como el anterior solo que get es la pagina principal y post es cuando el usuario procesa la comprar
def buy():
    if request.method == "POST":
        symbol = request.form.get("symbol").strip().upper()         # obtiene symbol (AAPL) .strip() borra los espacios y .upper() lo hace mayusculas
        shares_str = request.form.get("shares").strip()             # obtiene shares y .strip() borra los espacios

        if not symbol:
            return apology("Please enter a stock symbol.", 400)         # si el campo esta vacio osea no hay nada devuelve error

        stock = lookup(symbol)                                                  # usa lookup si el campo esta vacio osea no hay nada devuelve error -----------------------------------------------------------------------------
        if stock is None:
            return apology("Stock symbol not found.", 400)

        if not shares_str.isdigit() or int(shares_str) <= 0:                        #verifica que shares sea positivo
            return apology("Please enter a positive integer for shares.", 400)

        shares = int(shares_str)                #lo convierte a entero

        user_id = session["user_id"]        #obtiene el id en session
        user = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
        cash_available = user[0]["cash"]                                                #Consulta el saldo disponible en la base de datos.

        total_cost = shares * stock["price"]            #multiplica la cantidad de acciones por su precio

        if total_cost > cash_available:                                                     #si el costo total es mayor que el saldo marca error
            return apology("You do not have enough cash to complete the purchase.", 400)

        db.execute("UPDATE users SET cash = cash - ? WHERE id = ?", total_cost, user_id)        #resta y actualiza el dinero gastado en la compra los ? significan lo ultimo total y user

        db.execute("INSERT INTO transactions (user_id, symbol, shares, price, date) VALUES (?, ?, ?, ?, ?)",
                   user_id, symbol, shares, stock["price"], datetime.now().strftime("%Y-%m-%d %H:%M:%S"))

        flash("Purchase successful!")       # todo bien exito en la compra xd
        return redirect("/")                # redirige a index

    return render_template("buy.html")      # si el usuario ingresa con get muestra el formulario de compra

    return render_template("buy.html")


@app.route("/history")          #todo esto se ejecuta cuando este en la pagina de history
@login_required                 #necesita estar autentificado
def history():
    """Show user's transaction history."""
    user_id = session["user_id"]        # obtiene el user_id con la cuenta logeada

    # Ejecuta una consulta SQL para obtener solo las transacciones del usuario logeado, ordenadas por fecha de la más reciente a la más antigua.
    try:
        transactions = db.execute("""
            SELECT symbol, shares, price, date
            FROM transactions
            WHERE user_id = ?
            ORDER BY date DESC
        """, user_id)

        if transactions is None:
            transactions = []       # si no hay transacciones se crea una lista vacia para evitar errores

    except Exception as e:
        print(f"Error retrieving transactions: {e}")                # si hay errores muestra el error en la consola
        return "An error occurred while retrieving the history."        # Se produjo un error al recuperar el historial.

    return render_template("history.html", transactions=transactions)       # si hay transacciones simplemente pasa la lista


@app.route("/login", methods=["GET", "POST"]) #todo esto se ejecuta cuando este en la pagina de login
def login():
    """Log user in"""
    session.clear()
    if request.method == "POST":
        if not request.form.get("username"):
            return apology("must provide username", 403)
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):    # len !1 solo tiene que haber un usuario con ese nombre / check_password_hash funcion predefinida para comparar en este caso rows que es la contrasena encriptada y hash que es la contrasena que acaba de poner el usuario
            return apology("invalid username and/or password", 403)

        session["user_id"] = rows[0]["id"]      # autentidica y inicia sesion
        return redirect("/")                # una vez autenticado lo envia a la pagina principal
    else:
        return render_template("login.html")    # muestra el formulario para iniciar sesion


@app.route("/logout")
def logout():
    """Log user out"""
    session.clear()
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])           #todo esto se ejecuta cuando este en la pagina de quote
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol")     # obtine el simbolo ingresado por el usuario (AAPL)

        if not symbol:                                          #si no ingresa nada marca error
            return apology("must provide symbol", 400)

        stock = lookup(symbol)
        if stock is None:                                       # usa lookup si el campo esta vacio osea no hay nada devuelve error -----------------------------------------------------------------------------
            return apology("Invalid stock symbol", 400)

        return render_template("quoted.html", stock=stock)      # muestra quoted con la informacion de la accion

    return render_template("quote.html")            # muestra el get osea la pagina principal donde se coloca el simbolo


@app.route("/register", methods=["GET", "POST"])        #todo esto se ejecuta cuando este en la pagina de register
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")             # obtiene datos de el usuario
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        if not username:                                        # si no ingresa un nombre marca error
            return apology("must provide username", 400)

        if not password or not confirmation:                    # si no ingresa la contrasena o la confirmacion marca error
            return apology("must provide password", 400)

        if password != confirmation:                            # si la contrasena no es igual a la confirmacion marca error
            return apology("passwords must match", 400)

        try:
            db.execute("INSERT INTO users (username, hash) VALUES (?, ?)",
                       username, generate_password_hash(password))                  # guarda la contrasena en el lugar correspondiente
        except ValueError:                                                          # si el usuario ya ha sido registrado marca error   lo de antes el !=
            return apology("username already exists", 400)

        return redirect("/login")           # al registrarse redirige a login

    return render_template("register.html")         # si el usuario solo visita la pagina muestra el formulario de registro


@app.route("/sell", methods=["GET", "POST"])            #todo esto se ejecuta cuando este en la pagina de sell
def sell():
    if request.method == "POST":
        symbol = request.form.get("symbol")         #obtiene symbol
        if not symbol:                                                  #si no tiene symbol o esta vacio marca error
            return apology("You must select a stock.", 400)

        shares = request.form.get("shares")                             #obtiene shares
        if not shares.isdigit() or int(shares) <= 0:                        #verifica que shares sea positivo si no lo es marca error
            return apology("You must enter a positive integer.", 400)

        shares = int(shares)                                            # conviere shares en entero

        user_id = session["user_id"]                                    # obtiene el user_id con la cuenta logeada
        result = db.execute(
            "SELECT shares FROM transactions WHERE user_id = ? AND symbol = ?", user_id, symbol)        # verifica si tiene las acciones que va a vender

        total_shares = sum(row["shares"] for row in result)             # row toma shares y busca los mismo en results osea lo que obtubo en la base de datos arriba
        if shares > total_shares:                                               # si shares es mayor que total shares marca error
            return apology("You do not own enough shares to sell.", 400)

        stock = lookup(symbol)                                          # usa lookup si el campo esta vacio osea no hay nada devuelve error -----------------------------------------------------------------------------
        if stock is None:
            return apology("The stock symbol is not valid.", 400)

        sale_amount = shares * stock["price"]                           # el precio total se da (multiplicando shares la cantidad de acciones) y stock que es el precio de la accion

        db.execute("INSERT INTO transactions (user_id, symbol, shares, price, date) VALUES (?, ?, ?, ?, ?)",
                   user_id, symbol, -shares, stock["price"], datetime.now().strftime("%Y-%m-%d %H:%M:%S"))           #actualiza las compras ventas y toma el tiempo

        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", sale_amount, user_id)           #suma y actualiza sale amout ya que vendio se suma a el efectivo

        flash(f"You have sold {shares} shares of {symbol} at a price of {stock['price']} each. Total added to cash: {sale_amount}.")        # muestra informacion de la venta

        return redirect("/")

    stocks = db.execute(
        "SELECT DISTINCT symbol FROM transactions WHERE user_id = ?", session["user_id"])       # actuliza todas las ventas y compras del usuario
    return render_template("sell.html", stocks=stocks)                  # muestra el formulario de venta



@app.route("/change_password", methods=["GET", "POST"])
def change_password():
    if request.method == "POST":
        current_password = request.form.get("current_password")
        new_password = request.form.get("new_password")                 # obtiene los datos
        confirmation = request.form.get("confirmation")

        if not current_password or not new_password or not confirmation:        # si alguno de los datos estan vacios marca error
            return apology("All fields are required.", 400)

        if new_password != confirmation:                                        # si la nueva contrasena es diferente de la confirmacion amrca error
            return apology("New passwords must match.", 400)

        user_id = session["user_id"]                # identifica aal usuario
        rows = db.execute("SELECT hash FROM users WHERE id = ?", user_id)           # consulta la contrasena de la base de datos

        if not check_password_hash(rows[0]["hash"], current_password):              # compara ambas contrasenas si no son iguales marca error
            return apology("Current password is incorrect.", 403)

        db.execute("UPDATE users SET hash = ? WHERE id = ?",
                   generate_password_hash(new_password), user_id)                   # actualiza la nueva contrasena

        flash("Password changed successfully!")             # si todo sale bien marca exito
        return redirect("/")                                # redirige a la pagina principal

    return render_template("change_password.html")          # si el usuario entra a la pagina sin enviar el formulario simplemente lo muestra





#recuerda antes de ejecutar crear la tabla de las transacciones
