import os

from cs50 import SQL
from flask import Flask, redirect, render_template, request

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")


# Evita que el navegador almacene en caché una versión vieja de la página.
@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])    #obtener y actualizar
def index():
    if request.method == "POST":        #enviado y acutualizado
        # Obtiene datos del formulario
        name = request.form.get("name")
        month = request.form.get("month")
        day = request.form.get("day")

        # Validar datos y agregar a la base de datos
        if name and month and day:
            try:
                month = int(month)
                day = int(day)
                if 1 <= month <= 12 and 1 <= day <= 31:
                    db.execute("INSERT INTO birthdays (name, month, day) VALUES (?, ?, ?)", name, month, day)
            except ValueError:
                pass  # Ignorar errores si los datos no son válidos

        return redirect("/")        # Refresca la página para mostrar la lista actualizada de cumpleaños.

    else:
        # Consultar todos los cumpleaños de la base de datos
        birthdays = db.execute("SELECT * FROM birthdays")

        # Renderizar la plantilla con los datos
        return render_template("index.html", birthdays=birthdays)
