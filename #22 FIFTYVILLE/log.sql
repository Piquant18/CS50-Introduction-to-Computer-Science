-- Keep a log of any SQL queries you execute as you solve the mystery.
-- Requesting information from the crime scene database for the day of the robbery at the crime scene.
-- Robbery at 10:15 AM in the bakery HS, 3 witness interviews
SELECT description
FROM crime_scene_reports
WHERE month = 7 AND day = 28
AND street = 'Humphrey Street';


-- Requesting information from the witness interviews clave
SELECT transcript
FROM interviews
WHERE year = 2023 AND month = 7 AND day = 28;


-- Requesting information about the cars leaving the bakery between 10:15 AM - 10:25 AM
SELECT *
FROM bakery_security_logs
WHERE year = 2023 AND month = 7 AND day = 28
AND hour = 10 AND minute BETWEEN 15 AND 25;


-- License plates to cars leaving to bakery
SELECT p.name, p.phone_number, p.license_plate
FROM people AS p
WHERE p.license_plate IN (5P2BI95, 94KL13X, 6P58WS2, 4328GD8, G412CB7, L93JTIZ, 322W7JE, 0NTHK55);


-- Requesting information about ATM transactions
SELECT *
FROM atm_transactions
WHERE year = 2023 AND month = 7 AND day = 28
AND atm_location = 'Leggett Street';


-- Requesting information about bank accounts to identify people
SELECT p.name, p.phone_number, p.license_plate
FROM bank_accounts AS b
JOIN people AS p ON b.person_id = p.id
WHERE b.account_number IN (28500762, 28296815, 76054385, 49610011, 16153065, 86363979, 25506511, 81061156, 26013199);


-- Flights departing on the 29th
SELECT f.id, f.destination_airport_id, a.city
FROM flights AS f
JOIN airports AS a ON f.destination_airport_id = a.id
WHERE f.year = 2023 AND month = 7 AND day = 29;


-- Searching for passengers of flights one by one
SELECT p.name
FROM passengers AS ps
JOIN people AS p ON ps.passport_number = p.passport_number
WHERE ps.flight_id = 18;
23, 36, 43, 53


-- Checking the transactions of the 4 people
SELECT t.account_number, t.transaction_type, t.amount, t.year, t.month, t.day, t.atm_location
FROM atm_transactions AS t
WHERE t.account_number IN (
    SELECT b.account_number
    FROM bank_accounts AS b
    JOIN people AS p ON b.person_id = p.id
    WHERE p.name IN ('Iman', 'Luca', 'Diana', 'Bruce')
);


-- Flight information
SELECT f.id, f.origin_airport_id, f.destination_airport_id, f.year, f.month, f.day, f.hour, f.minute
FROM flights AS f
JOIN passengers AS ps ON f.id = ps.flight_id
JOIN people AS p ON ps.passport_number = p.passport_number
WHERE p.name IN ('Iman', 'Luca', 'Diana', 'Bruce');


-- Searching for the license plates of each person
SELECT p.name, v.license_plate
FROM people AS p
JOIN vehicles AS v ON p.id = v.person_id
WHERE p.name IN ('Iman', 'Luca', 'Diana', 'Bruce');


-- Only 2 people called for less than a minute, as mentioned in Bruce and Diana's interview
SELECT c.caller, c.receiver, c.year, c.month, c.day, c.duration
FROM phone_calls AS c
WHERE (c.caller IN ('(367) 555-5533', '(770) 555-1861', '(829) 555-5269', '(389) 555-5198')
   OR c.receiver IN ('(367) 555-5533', '(770) 555-1861', '(829) 555-5269', '(389) 555-5198'));


-- Information about Bruce and Diana's flights
SELECT flights.*, passengers.passport_number
FROM flights
JOIN passengers ON flights.id = passengers.flight_id
JOIN people ON passengers.passport_number = people.passport_number
WHERE people.phone_number IN ('(367) 555-5533', '(770) 555-1861')
ORDER BY year, month, day, hour, minute;


-- Confirming that the passports belong to Bruce and Diana
SELECT passport_number, name
FROM people
WHERE passport_number IN (5773159633 , 3592750733);


-- Investigating the first flight from Fiftyville (which was to New York) and who took it: Bruce
SELECT f.id, f.origin_airport_id, f.destination_airport_id, f.year, f.month, f.day, f.hour, f.minute, a.full_name AS origin_airport, b.full_name AS destination_airport
FROM flights f
JOIN airports a ON f.origin_airport_id = a.id
JOIN airports b ON f.destination_airport_id = b.id
WHERE f.year = 2023 AND f.month = 7 AND f.day = 29
AND a.city = 'Fiftyville'
ORDER BY f.hour, f.minute;


-- Investigating the phone number Bruce called to buy his flight ticket to escape to New York
-- This is the accomplice of the robbery
SELECT name
FROM people
WHERE phone_number = '(375) 555-8161';


