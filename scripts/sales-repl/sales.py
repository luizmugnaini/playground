"""
Author: Luiz Gustavo Mugnaini Anselmo

Implements a REPL for interactively construction of sale databases.
"""
import re
import csv
import json
import sys
from typing import Dict
import datetime as dt
import dataclasses
import logging


# Standard command prompt.
PROMPT = "\033[1;35m>>\033[0m"
# Standard error prompt.
ERROR = "\033[1;31m::\033[0m"
# Standard invalid variable message.
VAR_NAME_ERROR = f"{ERROR} Invalid variable name"


# Setting the logger
logger = logging.getLogger("sales")
handle = logging.StreamHandler(sys.stderr)
fmt = logging.Formatter("%(levelname)s - %(message)s")
handle.setFormatter(fmt)
logger.addHandler(handle)
logger.setLevel(logging.DEBUG)


@dataclasses.dataclass
class BRL:
    value: float


@dataclasses.dataclass
class USD:
    value: float


# Conversion rate from BRL to USD currency
USD_TO_BRL_RATE = 5.0


def brl_to_usd(brl_value: float) -> USD:
    """Convert a BRL value to USD currency"""
    return USD(brl_value / USD_TO_BRL_RATE)


def usd_to_brl(usd_value: float) -> BRL:
    """Convert a USD value to BRL currency"""
    return BRL(usd_value * USD_TO_BRL_RATE)


@dataclasses.dataclass
class Sale:
    product_name: str
    product_price: BRL | USD
    quantity: int
    client_cpf: int
    employee: str
    date: dt.datetime
    additional_info: str

    def __repr__(self):
        return repr(dataclasses.asdict(self))


class SalesDB:
    """Database of sales.

    Object holding the state of the database composed of `Sale` objects, identified by a monotonically
    increasing ID number.
    """

    __slots__ = "db"

    def __init__(self):
        self.db = dict()

    def insert(self, sale: Sale):
        id = self.last_id() + 1 if bool(self.db) else 1
        self.db[id] = sale

    def last_id(self) -> int:
        assert bool(self.db), "Empty database, can't retrieve the last sale ID."
        return max(self.db)

    def remove(self, *sale_ids):
        return [self.db.pop(id) for id in sale_ids]

    def view(self):
        return json.dumps(self.db, indent=4, default=str)

    def save_to_csv(self, filepath: str):
        with open(filepath, "w") as out:
            sale_fields = [field.name for field in dataclasses.fields(Sale)]
            writer = csv.DictWriter(out, fieldnames=["id", *sale_fields])
            writer.writeheader()

            for id, sale in self.db.items():
                sale = dataclasses.asdict(sale)
                sale["product_price"] = sale["product_price"]["value"]
                sale["date"] = sale["date"].strftime("%d/%m/%y")
                writer.writerow({"id": id, **sale})

    def to_usd(self):
        for sale in self.db.values():
            match sale.product_price:
                case BRL(val):
                    sale.product_price = brl_to_usd(val)

    def month_summary(self, month: int, year: int) -> tuple[float, float]:
        """
        Given a month and year, returns a tuple containing the total
        and average amount of money sold in that period.
        """
        n_sales = 0
        acc = 0.0
        for sale in self.db.values():
            date = sale.date
            if date.year == year and date.month == month:
                n_sales += 1
                acc += sale.product_price.value * sale.quantity
        if n_sales:
            return acc, acc / n_sales
        else:
            return 0.0, 0.0

    def employee_of_the_month(self, month: int, year: int) -> str | None:
        assert 1 <= month <= 12, f"Invalid month: {month}"
        employees: Dict[str, float] = dict()
        for sale in self.db.values():
            date = sale.date
            if date.year == year and date.month == month:
                name = sale.employee
                new_val = sale.product_price.value * sale.quantity
                employees[name] = employees.get(name, 0.0) + new_val
        if employees:
            return max(employees, key=lambda k: employees.get(k, 0.0))
        else:
            return None

    def product_of_the_month(self, month: int, year: int) -> str | None:
        assert 1 <= month <= 12, f"Invalid month: {month}"
        products: Dict[str, int] = dict()
        for sale in self.db.values():
            date = sale.date
            if date.year == year and date.month == month:
                name = sale.product_name
                products[name] = products.get(name, 0) + 1
        if products:
            return max(products, key=lambda k: products.get(k, 0))
        else:
            return None


class Query:
    """Query object.

    Handle a given raw query string, separating function or method name, considering arguments,
    variable names and comments.
    """

    __slots__ = ("fn_name", "var_name", "obj", "args")

    def __init__(self, raw_query: str, debug: bool = False):
        if debug:
            logger.debug(f"Creating query from '{raw_query}'")

        # Process the raw query
        if raw_query.lstrip()[0] == "#":
            return
        elif raw_query == "help" or raw_query == "exit":
            self.fn_name = raw_query
        elif "=" in raw_query:
            try:
                var_name, obj = raw_query.replace(" ", "").split("=")
                self.fn_name = "assignment"
                self.var_name = var_name
                self.obj = eval(obj)
            except ValueError as ve:
                raise ValueError(f"Incorrect assignment query: {ve}")
        elif "." in raw_query:
            try:
                # Preprocess the method string of the form "{method_name}({*args})", not affecting
                # any argument that is inside quotes.
                var_name, raw_method = raw_query.split(".", 1)
                raw_method = raw_method.replace(" ", "")
                raw_method = re.sub(r",|\(|\)", " ", raw_method)
                method_name, *args = raw_method.split()

                # Evaluate each string containing an argument, obtaining the object
                # corresponding to the argument itself.
                args = [eval(arg) for arg in args]

                self.fn_name, self.var_name, self.args = method_name, var_name, args
            except ValueError as ve:
                raise ValueError(f"Incorrect method usage: {ve}")

        if debug:
            logger.debug(repr(self))

    def __repr__(self):
        r = "Query {"
        for field in self.__slots__:
            r += f"{field}: {repr(getattr(self, field, None))}, "
        return r + "}"


class REPL:
    """REPL for creation and interaction with sales databases.

    Assignment
    ----------
    You can create as many variables as you want, but all of them must be a `SalesDB` object:
    >> x = SalesDB()
    >> y = SalesDB()

    Variable methods
    ----------------
    view : The view method allows the user to obtain an immediate visualisation of the object database.
        Suppose `x` is a variable of the REPL, a use case of `view` is:
        >> x.view()
    insert : The insert method allows the user to interactively insert a new sale to the object databese.
        Suppose `x` is a variable of the REPL, a use case of `insert` is:
        >> x.insert()
    remove : The remove method allows the user to remove arbitrarily many sale items from the object
        database by passing the sale ID. Suppose `x` is a variable of the REPL, a use case of `remove` is:
        >> x.remove(100)
        >> x.remove(4, 5, 9)
    save : The save method allows the user to save the object database to a csv file. Suppose `x` is a variable
        of the REPL, a use case of `save` is:
        >> x.save('database.csv')
    employee_of_the_month : The method allows the user to obtain the employee of the month, where the ranking
        is based on the total amount of money obtained by a given employee. Suppose `x` is a variable of
        the REPL, a use case of `employee_of_the_month` is:
        >> x.employee_of_the_month(5, 2023)
    month_summary : The method allows the user to obtain an a summary of the money obtained in a given
        month and year. Suppose `x` is a variable of the REPL, a use case of `month_summary` is:
        >> x.month_summary(4, 2020)
    product_of_month : The method allows the user to obtain the product most sold in a given
        month and year. Suppose `x` is a variable of the REPL, a use case of `product_of_month` is:
        >> x.product_of_month(12, 2022)
    """

    def __init__(self, debug: bool = False):
        print("Starting REPL...\nType 'help' for more information or 'exit' for quitting the REPL.")
        self.vars: Dict[str, SalesDB] = dict()
        self.debug = debug
        self.should_exit = False
        if self.debug:
            logger = logging.getLogger("repl")
            c_handle = logging.StreamHandler()
            fmt = logging.Formatter("%(levelname)s: %(message)s")
            c_handle.setFormatter(fmt)
            logger.addHandler(c_handle)
            logger.setLevel(logging.DEBUG)

    def assign_var(self, var_name: str, obj):
        """Create or set a variable of pertaining to the REPL state."""
        if self.debug:
            logger.debug(f"Setting variable: {{ {repr(var_name)}: {repr(obj)} }}")
        self.vars[var_name] = obj

    def is_var(self, var_name: str) -> bool:
        """Determine if the variable exists in the current REPL."""
        if self.debug:
            logger.debug(f"Checking if {repr(var_name)} is a valid variable")
        return var_name in self.vars

    def handle_var_method(self, var_name: str, method_name: str, args: list | None = None):
        """Handler for using methods associated to variables pertaining to the REPL."""
        if self.debug:
            logger.debug(
                "Handling method: "
                f"{{ var_name: {repr(var_name)}, method_name: {repr(method_name)}, args: {repr(args)} }}"
            )
        if self.is_var(var_name):
            try:
                method = getattr(self, method_name)
                if args:
                    return method(var_name, *args)
                else:
                    return method(var_name)
            except (ValueError, TypeError) as err:
                raise ValueError(f"{ERROR} Unable to run {repr(method_name)} due to error: {err}")
        else:
            print(VAR_NAME_ERROR, var_name)

    @staticmethod
    def _get_sale_field(name, cast_type):
        """Get the field value from stdin by interactively asking the user for information."""
        match name:
            case "product_price":
                try:
                    value = float(input(f"{PROMPT} \033[1;32mproduct_price\033[0m (default BRL): "))
                    return BRL(value)
                except ValueError as ve:
                    raise ValueError(f"Invalid price ({ve})")
            case "date":
                try:
                    d, m, y = re.split(
                        "/|-",
                        input(f"{PROMPT} \033[1;32m{name}\033[0m (dd/mm/yyyy or dd-mm-yyyy): "),
                    )
                    return dt.datetime(day=int(d), month=int(m), year=int(y))
                except ValueError as ve:
                    raise ValueError(f"Invalid date ({ve}).")
            case _:
                try:
                    return cast_type(input(f"{PROMPT} \033[1;32m{name}\033[0m: "))
                except ValueError:
                    raise ValueError(f"Invalid {repr(name)}, expected {cast_type}.")

    def insert(self, var_name: str):
        """Interactively insert an item to an existing variable."""
        print("Starting interactive sale insertion...")
        sale = dict()
        for name, cast_type in ((field.name, field.type) for field in dataclasses.fields(Sale)):
            try:
                # Get the user input from stdin
                sale[name] = REPL._get_sale_field(name, cast_type)
            except ValueError as ve:
                raise ValueError(f"Aborting insertion due to: {ve}")
        self.vars[var_name].insert(Sale(**sale))
        print("Item inserted successfully...")

    def _unsafe_insert(self, var_name: str, *args):
        """
        Unsafe method for directly inserting an item to a given variable database.
        Should only be used for testing purposes.
        """
        # Associate each argument to the corresponding field.
        fields = Sale.__dataclass_fields__.keys()
        dargs = dict(zip(fields, args))

        # Correction of the field values.
        dargs["product_price"] = BRL(dargs["product_price"])
        d, m, y = re.split("/|-", dargs["date"])
        dargs["date"] = dt.datetime(day=int(d), month=int(m), year=int(y))

        self.vars[var_name].insert(Sale(**dargs))

    def remove(self, var_name: str, *ids) -> str:
        """Remove a list of IDs of a variable."""
        items = self.vars[var_name].remove(*ids)
        return f"Successfully removed items:\n{items}"

    def view(self, var_name: str):
        """Visualise the contents of a given variable."""
        return self.vars[var_name].view()

    def save(self, var_name: str, filepath: str):
        """Save variable contents to a csv file."""
        self.vars[var_name].save_to_csv(filepath)

    # TODO: This should use a generator function.
    def to_usd(self, var_name: str):
        """Change variable price currency to USD."""
        self.vars[var_name].to_usd()

    def employee_of_the_month(self, var_name: str, month: int, year: int) -> str:
        """Get information about the employee of the month for a given month and year."""
        match self.vars[var_name].employee_of_the_month(month, year):
            case None:
                return f"Unable to retrieve employee of the month for {month}/{year}"
            case name:
                return f"The employee of the month is {name}"

    def product_of_the_month(self, var_name: str, month: int, year: int) -> str:
        """Get the information about the most sold product of a given month and year."""
        match self.vars[var_name].product_of_the_month(month, year):
            case None:
                return f"Unable to retrieve the most sold product of the month for {month}/{year}"
            case name:
                return f"The most sold product of the month is {name}"

    def month_summary(self, var_name: str, month: int, year: int) -> str:
        """Print the total and mean values obtained in a given month and year."""
        total, mean = self.vars[var_name].month_summary(month, year)
        return f"{month}/{year} summary: {{ total: {total}, mean: {mean} }}"

    def exit(self):
        """Put the REPL in exit state."""
        self.should_exit = True
        print("Exiting REPL...")

    def run_query(self, raw_query: str):
        """
        Given a raw query string from stdin, construct a `Query` object and perform the queried method
        or variable assignment.
        """
        query = Query(raw_query, debug=self.debug)
        # A query can be `None` when stdin is a comment, in such case we simply return and continue.
        if not query:
            return
        match query.fn_name:
            case "help":
                print(REPL.help())
            case "exit":
                self.exit()
            case "assignment":
                self.assign_var(query.var_name, query.obj)
            # Methods with no arguments.
            case "insert" | "to_usd" | "view" as m:
                try:
                    result = self.handle_var_method(query.var_name, m)
                    if result:
                        print(result)
                except (ValueError, TypeError, AssertionError) as err:
                    print(err)
            # Methods that require additional arguments.
            case "remove" | "save" | "_unsafe_insert" as m:
                try:
                    result = self.handle_var_method(query.var_name, m, query.args)
                    if result:
                        print(result)
                except (ValueError, TypeError, AssertionError) as err:
                    print(err)
            # Methods dealing with dates.
            case "month_summary" | "employee_of_the_month" | "product_of_the_month" as m:
                try:
                    month, year = query.args
                    result = self.handle_var_method(query.var_name, m, [int(month), int(year)])
                    print(result)
                except ValueError as ve:
                    print(f"{ERROR} Invalid date: {ve}")
            case _:
                print(
                    f"{ERROR} Invalid command {repr(raw_query)}."
                    " You can exit the REPL by typing 'exit' or ask for help by typing 'help'."
                )

    @staticmethod
    def _get_user_query() -> str:
        """Ask the user for a query and return the raw query."""
        return input(f"{PROMPT} ")

    @classmethod
    def run_repl(cls):
        """Runs the REPL loop."""
        repl = cls(debug=False)
        while not repl.should_exit:
            try:
                repl.run_query(cls._get_user_query())
            except KeyboardInterrupt:
                repl.exit()

    @staticmethod
    def help():
        """Print the documentation of the REPL for more instructions on how it can be used."""
        return REPL.__doc__


def test_repl():
    r = REPL(debug=True)
    queries = [
        "x = SalesDB()",
        "x.view()",
        # Both should error.
        "x.employee_of_the_month(2, 2004)",
        "x.month_summary(2, 2004)",
        # Insert some items.
        "x._unsafe_insert('orange', 4.0, 88, 111111111, 'Amanda', '06/05/1933', 'Fresh')",
        "x._unsafe_insert('mango', 6.0, 10, 81928319, 'Bob', '08/05/1933', 'Stale')",
        "x._unsafe_insert('keyboard', 133.0, 1, 12873917239, 'Amanda', '18/05/1933', 'Black')",
        "x.view()",
        # Saving to a filepath.
        "x.save('test.csv')",
        # Test grouping methods.
        "x.employee_of_the_month(5, 1933)",
        "x.month_summary(5, 1933)",
        "x.product_of_the_month(5, 1933)",
        # Test currency conversion.
        "x.to_usd()",
        "x.view()",
        # Running `to_usd` shouldn't change the value.
        "x.to_usd()",
        "x.view()",
        # Removal of a single item.
        "x.remove(2)",
        "x.view()",
        # Inserted item should have ID equal to 4.
        "x._unsafe_insert('puzzle', 13, 4, 1939812039, 'Bob', '09/02/1950', 'hard puzzle')",
        "x.view()",
        # Removal of multiple items.
        "x.remove(1, 3)",
        "x.view()",
    ]
    for q in queries:
        print(PROMPT, q)
        r.run_query(q)


def test_interactive_insertion():
    r = REPL()
    queries = ["x = SalesDB()", "x.insert()", "x.view()"]
    for q in queries:
        print(PROMPT, q)
        r.run_query(q)


def main():
    REPL.run_repl()


if __name__ == "__main__":
    if len(sys.argv) == 1:
        main()
    else:
        match sys.argv[1]:
            case "-t" | "--test":
                test_repl()
            case "-ti" | "--test-interactive":
                test_interactive_insertion()
