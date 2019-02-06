import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    portfolio = db.execute("SELECT * FROM portfolio WHERE userID = :userid", userid=session["user_id"])
    symbol = db.execute("SELECT symbol FROM portfolio WHERE userID = :userid", userid=session["user_id"])
    cash = db.execute("SELECT cash FROM users WHERE id = :userid", userid=session["user_id"])
    symbol_list = []
    numShares = db.execute("SELECT numShares FROM portfolio WHERE userID = :userid", userid=session["user_id"])

    sum_of_stocks = 0

    for s in symbol:
        symbol_list.append(s['symbol'])

    names = []

    prices = {}
    for symbol in symbol_list:
        stocks = lookup(symbol)
        prices[symbol] = stocks['price']
        names.append(stocks['name'])

    index = 0
    for price in prices.values():
        sum_of_stocks += (price * numShares[index]['numShares'])
        index += 1

    return render_template("index.html", portfolio=portfolio, names=names, prices=prices, cash=cash[0], sum_of_stocks=sum_of_stocks)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")

    if request.method == "POST":

        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)

        # Ensure num of shares is positive int
        if not request.form.get("shares") or int(request.form.get("shares")) < 1:
            return apology("number of shares must be positive", 400)

        # Look up stock
        stock = lookup(request.form.get("symbol"))
        price = stock['price']

        if not stock:
            return apology("Invalid stock symbol", 400)

        # Query database for cash in users account
        money = db.execute("SELECT cash FROM users WHERE id = :userid", userid=session["user_id"])
        cash = money[0]['cash']

        numShares = request.form.get("shares")
        total = float(price) * int(numShares)

        if cash > total:
            update_cash = db.execute("UPDATE users SET cash = cash - :total WHERE id = :userid",
            total=total, userid=session["user_id"])

        else:
            return apology("Sorry, you can't afford this tranaction.", 403)

        symbol = request.form.get("symbol")
        symbol = symbol.upper()

        # Below code borrowed from https://github.com/endiliey/cs50/blob/master/pset7/finance/application.py
        symbol_db = db.execute("SELECT symbol FROM portfolio WHERE userID = :userid AND symbol = :symbol", userid=session["user_id"], symbol=symbol)

        if not symbol_db:
            transaction = db.execute("INSERT INTO portfolio (userID, symbol, numShares, cost) VALUES (:userID, :symbol, :numShares, :cost)",
                userID=session["user_id"], symbol=symbol, numShares=int(numShares), cost=price)
            history = db.execute("INSERT INTO history (userID, symbol, numShares, type, price) VALUES (:userID, :symbol, :numShares, :type, :price)",
                userID=session["user_id"], symbol=symbol, numShares=int(numShares), type='Buy', price=price)
            return redirect("/")

        else:
            transaction = db.execute("UPDATE portfolio SET numShares = numShares + :num WHERE symbol = :symbol AND userID = :userid",
                symbol=symbol.upper(), userid=session["user_id"], num=int(numShares))
            history = db.execute("INSERT INTO history (userID, symbol, numShares, type, price) VALUES (:userID, :symbol, :numShares, 'Buy', :price)",
                userID=session["user_id"], symbol=symbol, numShares=int(numShares), price=price)
            return redirect("/")

        """
        This code doesn't work and I have no idea why! Preserving this for maybe when I'm better at coding.
        It always INSERTs and never UPDATEs. :(

        portfolio = db.execute("SELECT symbol FROM portfolio WHERE userID = :userid", userid=session["user_id"])

        for p in portfolio:
            if symbol in p.values():
                transaction = db.execute("UPDATE portfolio SET numShares = numShares + :num WHERE symbol = :symbol AND userID = :userid",
                symbol=symbol.upper(), userid=session["user_id"], num=int(numShares))
                return redirect("/")
            elif symbol not in p.values():
                transaction = db.execute("INSERT INTO portfolio (userID, symbol, numShares, cost) VALUES (:userID, :symbol, :numShares, :cost)",
                userID=session["user_id"], symbol=symbol.upper(), numShares=int(numShares), cost=price)
                return redirect("/")"""



@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    history = db.execute("SELECT * FROM history WHERE userID = :userid", userid=session["user_id"])

    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")
    if request.method == "POST":

        if not request.form.get("symbol"):
            return apology("Invalid symbol", 400)

        stock = lookup(request.form.get("symbol"))
        symbol = stock['symbol']
        name = stock['name']
        price = stock['price']
        return render_template("quoted.html", symbol=symbol, name=name, price=price)



@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Forget any user_id
    session.clear()

    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("Missing username!", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("Missing password!", 400)

        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("Passwords do not match.", 400)

        password = request.form.get("password")
        encrypted = generate_password_hash(password, method='pbkdf2:sha256', salt_length=8)

        # Insert username into database
        result = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)",
                          username=request.form.get("username"), hash=encrypted)

        # Ensure registration was successful
        if not result:
            return apology("invalid username and/or password", 400)

        # Remember which user has logged in
        session["user_id"] = result

        # Redirect user to home page
        return redirect("/login")

    # User reached page via "GET"
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "GET":

        symbols = db.execute("SELECT symbol FROM portfolio WHERE userID = :userid", userid=session["user_id"])

        return render_template("sell.html", symbols=symbols)

    if request.method == "POST":

        shares = int(request.form.get("shares"))

        numShares = db.execute("SELECT numShares FROM portfolio WHERE userID = :userid AND symbol = :symbol",
            symbol=request.form.get("symbol"), userid=session["user_id"])

        stock = lookup(request.form.get("symbol"))
        price = stock['price']

        if not request.form.get("symbol"):
            return apology("Must select a valid stock", 403)

        if not request.form.get("shares") or int(request.form.get("shares")) < 1:
            return apology("Must enter an appropriate amount of shares", 403)

        if shares < int(numShares[0]['numShares']): #TypeError: int() argument must be a string, a bytes-like object or a number, not 'list'
            sell = db.execute("UPDATE portfolio SET numShares = numShares - :shares WHERE symbol = :symbol AND userID = :userid",
                shares=shares, symbol=request.form.get("symbol"), userid=session["user_id"])
            history = db.execute("INSERT INTO history (userID, symbol, numShares, type, price) VALUES (:userID, :symbol, :numShares, 'Sell', :price)",
                userID=session["user_id"], symbol=request.form.get("symbol"), numShares=shares, price=price)

        elif shares == int(numShares[0]['numShares']):
            sell = db.execute("DELETE FROM portfolio WHERE numShares = :shares AND symbol = :symbol AND userID = :userid",
                shares=shares, symbol=request.form.get("symbol"), userid=session["user_id"])
            history = db.execute("INSERT INTO history (userID, symbol, numShares, type, price) VALUES (:userID, :symbol, :numShares, 'Sell', :price)",
                userID=session["user_id"], symbol=request.form.get("symbol"), numShares=shares, price=price)

        elif shares > int(numShares[0]['numShares']):
            return apology("You don't have that many shares, fool!", 403)


        total = float(price) * int(request.form.get("shares"))

        cash = db.execute("UPDATE users SET cash = cash + :total WHERE id = :userid",
            total=total, userid=session["user_id"])

        return redirect("/")

@app.route("/change", methods=["GET", "POST"])
@login_required
def change():
    """Change Password"""
    if request.method == "GET":
        return render_template("change.html")

    if request.method == "POST":

        password = request.form.get("password")
        encrypted = generate_password_hash(password, method='pbkdf2:sha256', salt_length=8)

        change = db.execute("UPDATE users SET hash = :hash WHERE id = :userid",
            hash=encrypted, userid=session["user_id"])

        return redirect("/")


def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)


# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
