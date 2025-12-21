# Professor Code System - Code Changes Documentation

## Overview
This document details all code changes made to implement the professor code system where admins assign a unique code that serves as both the username and password for professor accounts.

---

## Changes Made

### 1. **Professor Table Display - Added Code Column**

**File:** `UniManage/gui/adminpanel.cpp`

**Location:** Lines 351-355 (createProfessorsTab method)

**Change:** Modified the professors table to include a "Code" column

**Before:**
```cpp
m_professorsTable->setColumnCount(4);
m_professorsTable->setHorizontalHeaderLabels({"ID", "Professor Name", "Specialization", "Title"});
```

**After:**
```cpp
m_professorsTable->setColumnCount(5);
m_professorsTable->setHorizontalHeaderLabels({"ID", "Professor Name", "Code", "Specialization", "Title"});
```

**Purpose:** Added a new column to display the professor's login code in the admin dashboard.

---

### 2. **Professor Table Refresh - Display Code Data**

**File:** `UniManage/gui/adminpanel.cpp`

**Location:** Lines 525-536 (refreshProfessorsTable method)

**Change:** Updated table population to include the professor's National ID (which serves as their code) in the third column

**Before:**
```cpp
m_professorsTable->setItem(r, 2, new QTableWidgetItem(p.specialization()));
m_professorsTable->setItem(r, 3, new QTableWidgetItem(p.title()));
```

**After:**
```cpp
m_professorsTable->setItem(r, 2, new QTableWidgetItem(p.idNumber()));
m_professorsTable->setItem(r, 3, new QTableWidgetItem(p.specialization()));
m_professorsTable->setItem(r, 4, new QTableWidgetItem(p.title()));
```

**Purpose:** Populate the new Code column with the professor's identification number for admin reference.

---

### 3. **Add Professor Dialog - Professor Code Input**

**File:** `UniManage/gui/adminpanel.cpp`

**Location:** Lines 958-1035 (onAddProfessor method)

**Major Changes:**

#### Added Professor Code Input Field (Lines 964-966)
```cpp
QLineEdit* profCode = new QLineEdit();
profCode->setPlaceholderText("Enter unique professor code (e.g., PROF001)");
```

#### Added Informational Note (Lines 976-978)
```cpp
QLabel* noteLabel = new QLabel("Note: The Professor Code will be used as the login password.");
noteLabel->setStyleSheet("color: #666; font-style: italic; font-size: 11px;");
layout->addRow(noteLabel);
```

#### Changed User Account Creation (Lines 987-991)
**Before:**
```cpp
u.setUsername(idNum->text()); 
u.setPassword("prof123");
```

**After:**
```cpp
u.setUsername(profCode->text()); 
u.setPassword(profCode->text()); // Password is the professor code
```

#### Updated Username Lookup (Line 997)
**Before:**
```cpp
cu = m_userController.getUserByUsername(idNum->text());
```

**After:**
```cpp
cu = m_userController.getUserByUsername(profCode->text());
```

#### Enhanced Success Message (Lines 1019-1027)
**Before:**
```cpp
QMessageBox::information(this, "Success", "Professor registered successfully (Linked to existing identity).");
```

**After:**
```cpp
QString successMsg = QString("Professor registered successfully!\n\n"
                            "Login Credentials:\n"
                            "Username: %1\n"
                            "Password: %2\n\n"
                            "Please provide these credentials to the professor.")
                            .arg(profCode->text())
                            .arg(profCode->text());
QMessageBox::information(this, "Success", successMsg);
```

**Purpose:** 
- Allow admin to specify a unique professor code during registration
- Use this code as both username and password
- Display the credentials to the admin after successful registration

---

### 4. **Edit Professor Dialog - Code Modification**

**File:** `UniManage/gui/adminpanel.cpp`

**Location:** Lines 1037-1108 (onEditProfessor method)

**Major Changes:**

#### Retrieve Current Professor Code (Lines 1060-1062)
```cpp
User currentUser = m_userController.getUserById(p.userId());
QString currentCode = currentUser.username();
```

#### Added Professor Code Edit Field (Lines 1068-1070)
```cpp
QLineEdit* profCode = new QLineEdit(currentCode);
profCode->setPlaceholderText("Professor login code");
```

#### Added Informational Note (Lines 1078-1080)
```cpp
QLabel* noteLabel = new QLabel("Note: Changing the code will update the login username and password.");
noteLabel->setStyleSheet("color: #666; font-style: italic; font-size: 11px;");
layout->addRow(noteLabel);
```

#### Update User Credentials When Code Changes (Lines 1095-1100)
```cpp
bool codeChanged = (profCode->text() != currentCode);
if (codeChanged) {
    currentUser.setUsername(profCode->text());
    currentUser.setPassword(profCode->text()); // Password matches code
}
currentUser.setFullName(name->text());
```

#### Enhanced Success Message (Lines 1102-1107)
```cpp
if (m_userController.updateUser(currentUser) && m_professorController.updateProfessor(p)) {
    QString msg = "Professor records updated.";
    if (codeChanged) {
        msg += QString("\n\nNew Login Credentials:\nUsername: %1\nPassword: %2")
               .arg(profCode->text()).arg(profCode->text());
    }
    QMessageBox::information(this, "Success", msg);
```

**Purpose:** 
- Allow admin to view and modify the professor's login code
- Automatically update both username and password when code is changed
- Notify admin of new credentials if code was modified

---

## Summary of Functionality

### For Admins:
1. **Registration**: When adding a new professor, admin enters a unique "Professor Code" which becomes the login credentials
2. **Viewing**: The professor's code is displayed in the "Code" column of the professors table
3. **Editing**: Admin can modify the professor code, which automatically updates login credentials
4. **Credentials Display**: After registration or code change, admin receives a message showing the username and password to share with the professor

### For Professors:
1. **Login**: Professors use their assigned code as both username and password
2. **Security**: The code is set by the admin and can be changed if needed

---

## Files Modified

1. **UniManage/gui/adminpanel.cpp**
   - Lines 351-355: Table column definition
   - Lines 525-536: Table data population
   - Lines 958-1035: Add professor dialog and logic
   - Lines 1037-1108: Edit professor dialog and logic

---

## Testing Recommendations

1. **Add Professor**: Verify that entering a professor code creates an account with matching username/password
2. **View Code**: Check that the code appears in the professors table
3. **Edit Code**: Confirm that changing the code updates login credentials
4. **Login**: Test that professors can log in using their assigned code
5. **Duplicate Prevention**: Ensure duplicate codes are rejected

---

*Document generated: 2025-12-21*
*System: UniManage - University Management System*
