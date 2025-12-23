# UniManage Project – Technology Overview

## 📦 Project Overview – What’s Under the Hood?

| Area | Technology / Library | What It Does in **UniManage** | Typical Tags / Keywords you’ll see |
|------|----------------------|------------------------------|-----------------------------------|
| **Language** | **C++ (C++17)** | Core business logic, models, controllers, and the Qt‑based UI. | `#include`, `class`, `public:`, `private:`, `virtual`, `override` |
| **GUI Framework** | **Qt 5/Qt 6** (Qt Widgets) | Provides all windows, dialogs, tables, combo‑boxes, layouts, signals/slots, and the event loop that powers the Administration, Professor, and Student dashboards. | `QWidget`, `QDialog`, `QTableWidget`, `QComboBox`, `QPushButton`, `QVBoxLayout`, `QHBoxLayout`, `QFormLayout`, `QHeaderView`, `Q_OBJECT`, `signals:`, `slots:` |
| **Database** | **MySQL** (accessed via **Qt Sql** with the `QMYSQL` driver) | Stores users, students, courses, enrollments, rooms, schedules, etc. Connection parameters are read from environment variables (`DB_HOST`, `DB_PORT`, `DB_USER`, `DB_PASSWORD`, `DB_NAME`). The singleton `DBConnection` creates the database if missing and runs schema migrations on startup. | `QSqlDatabase`, `QSqlQuery`, `QSqlError`, `SELECT_…`, `INSERT_…`, `UPDATE_…`, `DELETE_…` (see `database/queries.h`) |
| **Build System** | **qmake** (or **CMake** – the repo contains a `CMakeLists.txt` if you prefer) | Generates the Makefiles / Visual‑Studio project that compiles the C++/Qt sources and links the Qt libraries. | `QT += widgets sql`, `CONFIG += c++17`, `TARGET = UniManage` |
| **Version‑Control** | **Git** (standard `.gitignore`, `git` commands) | Tracks source files, migrations, and documentation. | `git commit`, `git push`, `git pull` |
| **Documentation** | **Markdown** (`*.md` files) | Project README, workflow guides, and the “exam” description. | `#`, `##`, `-`, back‑ticks for code snippets |
| **Testing / Validation** | **Qt Test (QTestLib)** – used in the *exam* harness to run unit‑style checks on controllers/models. | Verifies that controllers correctly talk to the DB, that UI logic updates tables, etc. | `QTEST_MAIN`, `QVERIFY`, `QCOMPARE` |
| **Utility / Helper** | **StyleHelper** (custom class) | Centralizes the application stylesheet (CSS‑like Qt style) for a consistent look‑and‑feel. | `StyleHelper::getMainStyle()` |
| **Other C++ Libraries** | **Standard Library** (`<algorithm>`, `<vector>`, `<map>`, `<regex>`) | Used for sorting, filtering, containers, and regular‑expression validation. | `std::sort`, `std::find_if`, `std::regex` |
| **Exam‑Specific Scripts** | **SQL migration files** (`*.sql`) | Set up the initial schema, seed data, and clean‑up orphaned records for the exam environment. | `CREATE TABLE`, `INSERT INTO`, `ALTER TABLE` |
| **Packaging / Distribution** | **NSIS** (Windows installer) – optional, used to bundle the compiled binary with the SQLite DB for the exam delivery. | Generates a single `.exe` installer for Windows. | `!include`, `Section`, `File` directives in `.nsi` files |

---

## 🔎 How the Main Pieces Fit Together

1. **Qt Widgets** – All UI screens (`AdminPanel`, `ProfessorPanel`, `StudentPanel`, etc.) inherit from `QWidget`.  
   *Signals* (e.g., `clicked()`) and *slots* (e.g., `onAddStudent()`) connect UI actions to controller logic.
2. **Controllers** – Thin C++ classes that encapsulate DB operations.  
   Example: `StudentController::addStudent(const StudentData&)` builds a `QSqlQuery` using the SQL strings from `database/queries.h`.
3. **Models** – Plain‑old‑data‑structures (`StudentData`, `Course`, `AcademicLevel`, …) that hold fields and provide getters/setters.  
   They are passed between UI and controllers.
4. **Database Layer** – A single SQLite file (`UniManage.db`) lives in the project root.  
   Qt’s `QSqlDatabase` opens it (`DBConnection::instance().database()`) and all queries go through `QSqlQuery`.
5. **Styling** – `StyleHelper` returns a stylesheet string that gives the app a modern, dark‑mode‑friendly look (gradients, rounded buttons, custom object names like `primaryBtn`, `dangerBtn`).
6. **Exam Harness** – The **exam** uses the same codebase but runs a suite of Qt Test cases that automatically:
   * Populate the DB with test data (`exam_data.sql`).
   * Call controller methods to verify business rules (e.g., “Year/Level must be 1‑5”).
   * Check that UI tables display the expected columns (e.g., “Year/Level” appears).

---

## 🏷️ Common Qt‑Specific Tags / Macros You’ll See

| Tag / Macro | Meaning / Usage |
|-------------|-----------------|
| `Q_OBJECT` | Enables meta‑object features (signals/slots, `QObject::tr()`, dynamic properties). |
| `signals:` / `slots:` | Declare signal emitters and slot receivers for event‑driven communication. |
| `connect(sender, &QPushButton::clicked, this, &AdminPanel::onAddStudent);` | Wire UI events to handler functions. |
| `QSqlQuery::prepare()` | Parameterised SQL statements to avoid injection. |
| `QRegularExpressionValidator` | Enforces input formats (e.g., 14‑digit National ID). |
| `QHeaderView::setSectionResizeMode(QHeaderView::Stretch)` | Makes table columns auto‑stretch to fill the view. |
| `QMessageBox::warning/info/critical` | Simple modal dialogs for user feedback. |
| `QDialog::exec()` | Modal dialog pattern for Add/Edit forms. |
| `QComboBox::addItem(text, userData)` | Populate dropdowns; `userData` is the DB ID. |
| `QSpinBox::setRange(min, max)` | Constrain numeric inputs (e.g., academic level 1‑5). |
| `QDoubleSpinBox` | For monetary values like tuition fees. |
| `Q_DECLARE_METATYPE` (rare) | Makes custom types usable in Qt’s meta‑object system (e.g., for `QVariant`). |
| `Q_PROPERTY` (rare) | Exposes class members to Qt Designer / QML (not heavily used here). |

---

## 📚 “Other Technologies” Mentioned in the Exam Context

| Component | Why It’s Part of the Exam |
|-----------|---------------------------|
| **SQLite** | Lightweight, file‑based DB that can be shipped with the exam without a server. |
| **Qt Test (QTestLib)** | Provides a deterministic way to verify that the student’s implementation meets the spec (e.g., correct filtering, proper DB updates). |
| **SQL Migration Scripts** (`*.sql`) | Used to set up the initial schema and seed data before the exam starts. |
| **Markdown Documentation** (`README.md`, `EXAM_GUIDE.md`) | Gives candidates the specification and grading rubric. |
| **Git** | The exam repository is cloned; version control ensures a clean starting point. |
| **CMake / qmake** | The build system that compiles the solution on the exam machines (both are supported). |
| **NSIS Installer** (optional) | If the exam is delivered as a Windows installer, NSIS packages the binary + DB together. |

---

### TL;DR
- **Core language**: C++ 17
- **UI**: Qt Widgets (`QWidget`, `QTableWidget`, `QComboBox`, …) with signals/slots.
- **Persistence**: SQLite via Qt Sql (`QSqlDatabase`, `QSqlQuery`).
- **Build**: qmake/CMake, linking against `Qt5/Qt6Widgets` and `Qt5/Qt6Sql`.
- **Styling**: Centralized stylesheet (`StyleHelper`).
- **Testing (exam)**: Qt Test framework plus SQL seed scripts.

All of these pieces together give you a full‑stack desktop application for managing a university’s students, courses, professors, rooms, schedules, and academic levels—exactly what the exam expects you to understand and extend.

---
