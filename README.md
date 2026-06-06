# WorkSphere — Employee Record Management System

A console-based employee records utility written in C++. Built to practice object-oriented design,
persistent file storage, and practical backend-style CRUD logic.

---

## Overview

WorkSphere is a command-line application for managing employee data within an organization.
It supports adding, viewing, updating, and deleting employee records; searching across multiple
fields; and sorting results by salary, name, join date, or performance rating.

Data is stored in a pipe-delimited flat file and automatically reloaded on startup, so no
database dependency is needed. The whole thing compiles with a single `make` command.

---

## Why I Built This

I wanted a project that went beyond the usual calculator or to-do list.
My goals were to:

- Get more comfortable with class design and separating responsibilities across files
- Actually implement file persistence properly — not just print to the console and pretend
- Practice using STL algorithms (sorting with custom comparators felt surprisingly tricky at first)
- Build something I could realistically explain in an interview

The employee management domain felt like a good fit because the data model is rich enough
to be interesting without needing a real backend.

---

## Technical Decisions

**File format — pipe-delimited flat file**

I initially considered binary serialization, but plain text makes the data file inspectable
and easier to debug when something goes wrong. I went with `|` as the delimiter instead of
commas because names and addresses can contain commas. Backslash escaping handles the rare
case where a `|` appears in a field value.

**Saving on every write**

Rather than batching writes, the manager saves the full file after every add/update/delete.
It's slightly inefficient for large datasets, but for this scale it keeps the persistence
logic simple and avoids the risk of data loss if the process is killed mid-session.

**Vectors over maps**

I store employees in a `std::vector` rather than a `std::map<string, Employee>`. Linear
scan for lookups is fine at this scale, and vectors are easier to pass around and sort
with `std::sort`. If this were handling tens of thousands of records I'd reconsider.

**No external libraries**

Everything compiles with standard C++17. No Boost, no SQLite, no third-party headers.
This keeps setup friction to zero and makes the dependency story for a portfolio project clean.

**Namespace-based utilities**

`Validation` and `Display` are implemented as namespaces rather than classes. They don't
hold state — they're just collections of related functions. Wrapping them in a class with
all-static methods felt forced.

---

## Architecture

```
WorkSphere/
├── include/
│   ├── Employee.h          # Data model — fields + CSV serialization
│   ├── EmployeeManager.h   # Owns the employee list, handles CRUD + search/sort
│   ├── Validation.h        # Input validation functions
│   └── Display.h           # Console formatting and menus
├── src/
│   ├── main.cpp            # Menu loop and user interaction
│   ├── Employee.cpp
│   ├── EmployeeManager.cpp
│   ├── Validation.cpp
│   └── Display.cpp
├── data/
│   └── employees.dat       # Persisted employee records
└── Makefile
```

**Employee** — plain data class. Knows how to serialize itself to and from a CSV line.
No business logic here.

**EmployeeManager** — owns the in-memory `vector<Employee>` and the file path.
All mutations go through this class, which ensures the file stays in sync. Search methods
return raw pointers into the vector (safe since the manager owns the vector's lifetime
within a single session).

**Validation** — stateless input checking. Each function returns a bool and, for numeric
types, writes the parsed value via an output parameter. This keeps error handling in the
caller where it can give the user meaningful feedback.

**Display** — all the console output lives here. Separating it from logic meant I could
tweak column widths and formatting without touching any business code.

**main.cpp** — just the menu loop and individual feature handlers. Each feature is its
own static function so `main()` itself stays readable.

---

## Features

### CRUD

- **Add** — collects all fields with per-field validation, checks for duplicate ID,
  confirms before writing
- **View All** — formatted table with truncation for long designation strings
- **Update** — shows current values in brackets; pressing Enter skips a field
- **Delete** — shows the full record before asking for confirmation

### Persistent Storage

Records are stored in `data/employees.dat` as pipe-delimited lines with backslash escaping.
The file is read on startup and rewritten in full after any mutation. A comment line at
the top of the file marks it as managed by the application.

### Search

Searches by ID (exact), name (substring), department (substring), and designation (substring).
All substring searches are case-insensitive using `std::transform` + `string::find`.

### Sorting

Uses `std::sort` with a lambda comparator. Supports ascending and descending order for
name (lexicographic), salary (numeric), joining date (lexicographic on YYYY-MM-DD, which
sorts correctly by construction), and performance rating (numeric).

### Validation

| Field          | Rule |
|----------------|------|
| Employee ID    | 3–12 alphanumeric chars, hyphens and underscores allowed |
| Name           | 2–60 chars, letters/spaces/hyphens/apostrophes only |
| Salary         | Non-negative number, max $10,000,000 |
| Performance    | Float between 0.0 and 5.0 |
| Joining Date   | Must parse as YYYY-MM-DD |
| Phone          | 7–15 chars, digits + `+`, `-`, spaces, parentheses |
| Email          | Must contain `@` with a dot after it |

---

## Setup

### Requirements

- g++ with C++17 support (GCC 7+ or Clang 5+)
- Make

### Build

```bash
git clone https://github.com/yourusername/WorkSphere.git
cd WorkSphere
make
```

### Run

```bash
./worksphere
```

The application looks for `data/employees.dat` relative to the working directory.
Run from the project root. On first run with no data file, the system starts empty.
The sample data file in `data/` has 10 pre-populated records.

### Clean

```bash
make clean
```

---

## Sample Session

```
---------------------------------------------------------------------------------------------------------
                       WorkSphere  —  Employee Record System
---------------------------------------------------------------------------------------------------------

    1.  Add New Employee
    2.  View All Employees
    3.  Search Employees
    ...

  Select option: 3

  Search by:
    1. Employee ID
    2. Name
    3. Department
    4. Designation

  Select: 3
  Department contains: Engineering

  ID        Name                  Department    Designation           Salary  ...
  EMP-001   Alice Hartman         Engineering   Senior Software...    $112000
  EMP-002   Marcus Webb           Engineering   Backend Developer     $88500
  ...
```

---

## Challenges

**Keeping the file consistent across updates**

The naive approach — read everything in, mutate in memory, write everything back — works
but means a crash during a write could leave a corrupt file. I didn't implement atomic
file replacement (write to a temp file, then rename) because it felt over-engineered for
this scope, but that would be the right solution in production.

**Handling partially entered records**

The add workflow collects fields one at a time. If the user types "cancel" midway through,
nothing should be written. I handled this by collecting all inputs first and only calling
`addEmployee()` after getting confirmation, so there's no partial-write problem.

**Sorting dates stored as strings**

Joining dates are stored as strings in YYYY-MM-DD format. Sorting them lexicographically
happens to give correct chronological order — but only because I validated the format
on input. If someone fed in a date like "Jan 2022" it would sort incorrectly. The format
validation at input time is what makes the sort work.

**Substring search UX**

I wanted search to feel usable rather than requiring exact full-string matches.
Using `std::transform` to lowercase both the haystack and needle before calling
`string::find` was the cleanest approach I found.

---

## Future Improvements

Some things I'd do differently or add with more time:

- **SQLite backend** — the flat file approach is fine for learning, but a real tool
  would use a proper database. SQLite would be the obvious choice for a self-contained
  C++ app.

- **Export to CSV / JSON** — useful for reporting or importing into a spreadsheet.

- **Pagination** — the table view dumps all records at once. With a large dataset
  you'd want page-based navigation.

- **Authentication** — even basic role-based access (admin vs. read-only) would make
  the tool more realistic as an internal utility.

- **Undo / change history** — right now deletes and updates are immediate and permanent.
  A simple command stack for undo would be useful.

- **GUI version** — I'm curious how this would translate to something like Qt or ImGui.
  The separation of Display logic from business logic was partly motivated by keeping that
  door open.

---

## Reflection

The part I found most interesting was working out the file persistence. It sounds simple
but there are a bunch of edge cases: what if a field contains the delimiter character,
what if the file is partially written, what if a line has the wrong number of fields.
Dealing with those forced me to think more carefully about the data format than I expected.

Designing the class structure also took a few iterations. My first draft had more in
`main.cpp` than it should have. Moving the feature handlers into separate functions and
getting the `EmployeeManager` to own all mutations (rather than having `main.cpp` reach
into the vector directly) made the code much easier to follow.

If I were starting over, I'd probably use a proper date type from the start rather than
storing dates as strings and relying on format consistency for sorting. It works, but it's
a bit fragile.


