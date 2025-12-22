# C++ Concepts Applied to University Management System Scenario

This document maps fundamental programming concepts to the implemented **UniManage** Student Management System.

## 1. Arrays, Vectors, and Structures (P7)

### Arrays
**Concept**: An array is a collection of elements of the same type placed in contiguous memory locations. They have a fixed size.
**In Scenario**: 
While this project primarily uses dynamic containers (Qt), the concept of an array is represented by fixed sets of data. 
*   **Example**: The `headers` list in `ProfessorPanel::createAttendanceTab` acts like an array of strings defining column titles.
*   **Hypothetical**: `int grades[5];` could hold the fixed 5 attributes of a student's performance (Ass1, Ass2, CW, Final, Total).

### Vectors (Dynamic Arrays)
**Concept**: Vectors are dynamic arrays that can resize automatically when elements are added or removed. In this Qt-based project, `QList` and `QVector` serve this purpose.
**In Scenario**: 
`QList<Course>` and `QList<Enrollment>` are the backbone of the system's data management.
*   **Example**: `m_allProfessorCourses` in `ProfessorPanel` is a dynamic list that grows depending on how many courses are assigned to a professor.
    ```cpp
    QList<Course> list; // A vector-like container
    list.append(c);     // Dynamically adding a course
    ```

### Structures
**Concept**: A structure (`struct`) is a user-defined data type that groups variables of different data types under a single name.
**In Scenario**:
We use `struct` (and `class` which is similar but defaults to private access) to model complex entities.
*   **Example**: `RoomSpec` in `room.h` is a `struct` defining the specific equipment in a room.
    ```cpp
    struct RoomSpec {
        int id;
        int roomId;
        QString productName; 
    };
    ```
    It groups an integer ID with a string Name, creating a single unit of data.

---

## 2. Functions in C++ (P8)

**Concept**: Functions are blocks of code designed to perform a specific task. They provide modularity and code reuse.

### Types of Functions in Scenario:

1.  **Member Functions (Methods)**:
    *   **Description**: Functions belonging to a class.
    *   **Snippet**: `StudentData::fullName()` logic combines first and last names.
    *   **Use**: Encapsulation of behavior related to an object.

2.  **Void Functions (Actions)**:
    *   **Description**: Functions that perform an action but return no value.
    *   **Snippet**: `void ProfessorPanel::onSaveGrades()`
    *   **Use**: Triggering main logic like saving data to the database without needing a result value.

3.  **Value-Returning Functions**:
    *   **Description**: Functions that calculate and return data.
    *   **Snippet**: `double Enrollment::totalGrade()`
    *   **Use**: Computing the sum of assignment and exam marks.

4.  **Parameterised Functions**:
    *   **Description**: Functions accepting inputs.
    *   **Snippet**: `getStudentById(int id)`
    *   **Use**: Fetching specific data based on input criteria.

---

## 3. Pointers in C++ (P9)

**Concept**: A pointer is a variable that stores the memory address of another variable. They are essential for dynamic memory allocation and efficient object passing.

**In Scenario**:
We use pointers heavily for UI elements and Controller interactions.

*   **Example**:
    ```cpp
    QTableWidgetItem* nameItem = new QTableWidgetItem(e.studentName());
    ```
    *   `QTableWidgetItem*` is a pointer.
    *   `new` allocates memory on the heap for this item.
    *   The pointer `nameItem` stores the address of this new object.

*   **Use Case**: In `ProfessorPanel`, `m_studentsTable` is a pointer to a `QTableWidget`. We pass this pointer around or use arrow notation (`->`) to access its methods (e.g., `m_studentsTable->setItem(...)`), manipulating the actual table object in memory rather than a copy.

---

## 4. Compare Arrays, Vectors, and Structures (M3)

| Feature | Arrays | Vectors (`QList`/`std::vector`) | Structures (`struct`/`class`) |
| :--- | :--- | :--- | :--- |
| **Data Type** | Homogeneous (Same type) | Homogeneous (Same type) | Heterogeneous (Different types) |
| **Size** | Fixed at compile-time (Static) | Dynamic (Resizes at runtime) | Fixed Layout (Defined fields) |
| **Memory** | Contiguous Stack/Global | Contiguous Heap | Contiguous Stack/Heap |
| **Scenario Use** | `QStringList headers` (UI Labels) | `QList<Enrollment>` (Student lists) | `StudentData`, `RoomSpec` (Models) |
| **Flexibility** | Low (Cannot grow) | High (Can `append`/`remove`) | N/A (Defines shape of data) |

**Key Difference**: 
In the **Professor Dashboard**, we use a **Structure** (`Enrollment` class) to define *what* a student record looks like (Name + Grade + Status). We use a **Vector** (`QList<Enrollment>`) to store the *list* of these records because we don't know how many students are in a class. We might use an **Array** conceptually for the fixed number of days in a week (7) for the schedule.

---

## 5. Real-World C++ Program Implementation (D3)

The **UniManage** system itself is the implementation of this requirement.

*   **Functions**: `EnrollmentController::calculateTotalAndGrade` is a modular function that processes raw numbers into a grade.
*   **Vectors**: `QList<Course> courses` stores the variable number of courses a professor teaches.
*   **Structures/Classes**: `Course` object holds mixed data (ID `int`, Name `QString`, Description `QString`).
*   **Arrays**: Used implicitly in the database result sets (rows and columns).

**Example Workflow Code**:
```cpp
// 1. Structure/Class Definition
class Enrollment {
    QString m_studentName;
    double m_totalGrade;
    // ...
};

// 2. Vector of Structures
QList<Enrollment> enrollments; 

// 3. Function processing
void calculateGrades() {
    for(int i = 0; i < enrollments.size(); ++i) { // interacting with vector
        double total = enrollments[i].assignment1() + enrollments[i].final();
        enrollments[i].setTotal(total);
    }
}
```

---

## 6. File Handling in Python (P10)

**Concept**: Python provides built-in functions to create, read, update, and delete files. This is crucial in real-world apps for logs, exports, and data persistence.

**Application to Scenario**:
Although UniManage is C++, a Python script could be used to **generate report cards** from exported data.

**Example Scenario**: A backend script reading a CSV of grades exported by the C++ app.

```python
# python_grade_processor.py

# 1. Opening a file (Concept: File Access)
try:
    with open("student_data_export.csv", "r") as file:
        # 2. Reading contents line by line
        for line in file:
            data = line.strip().split(",")
            student_name = data[1]
            total_grade = float(data[5])
            
            # 3. Processing
            status = "Pass" if total_grade >= 60 else "Fail"
            print(f"Student: {student_name}, Status: {status}")
            
except FileNotFoundError:
    print("Export file not found!")
```

**Importance**:
1.  **Persistence**: Storing data when the DB is unavailable.
2.  **Interoperability**: Moving data between the C++ System and other tools (Excel, Web).
3.  **Logging**: Writing error logs (e.g., "Failed to save grade for ID 101") for auditing.

---

## 7. Classes and Objects in C++ (P11)

### Class
**Concept**: A blueprint or template for creating objects. It defines member variables (attributes) and member functions (behaviors).
**In Scenario**: `Course` is a class.
*   It defines *properties* like `m_name`, `m_creditHours`.
*   It defines *behaviors* like `setSemesterYear()`.

### Object
**Concept**: An instance of a class. It is a concrete entity in memory.
**In Scenario**: 
When `ProfessorPanel` loads, we create specific objects:
*   `Course cppCourse;` is an object.
*   `cppCourse.setName("C++ Programming");` sets the state of this specific object.

**Example**:
```cpp
// Class Definition (Blueprint)
class Professor {
    QString m_name;
public:
    void setName(QString n) { m_name = n; }
};

// Object Creation (Real Entity)
Professor profAdam; 
profAdam.setName("Adam Ahmed"); // Interacting with the object
```
