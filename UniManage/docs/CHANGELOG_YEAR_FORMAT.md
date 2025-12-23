# Changelog: Academic Year Format Update

**Date**: December 23, 2025  
**Summary**: Changed all Academic Level displays from custom names (e.g., "Freshman", "Sophomore") to standardized "Year X" format throughout the application.

## Changes Made

### 1. Professor Dashboard (`gui/professorpanel.cpp`)
- **Year Selector**: Now displays "Year 1", "Year 2", etc. instead of custom level names
- **Location**: `loadCourses()` function, line ~352
- **Impact**: Professors now see consistent year numbering when filtering students

### 2. Admin Panel - Course Management (`gui/adminpanel.cpp`)

#### Course Creation Form
- **Academic Level Dropdown**: Changed from `l.name()` to `QString("Year %1").arg(l.levelNumber())`
- **Location**: `onAddCourse()`, line ~1377
- **Display**: "Year 1", "Year 2", "Year 3", "Year 4", "Year 5"

#### Course Edit Form
- **Academic Level Dropdown**: Updated to "Year X" format
- **Location**: `onEditCourse()`, line ~1461-1467
- **Fix Applied**: Changed from unreliable `setCurrentIndex(count()-1)` to proper `findData()` lookup
- **Impact**: Edit form now correctly pre-selects the current year level

#### Course Table Display
- **Level Column**: Shows "Year X" instead of custom names
- **Location**: `refreshCoursesTable()`, line ~1025
- **Implementation**: Uses QMap lookup with formatted year numbers

### 3. Admin Panel - Student Management

#### Student Registration Form
- **Academic Year Dropdown**: Updated to "Year X" format
- **Location**: `onAddStudent()`, line ~1165
- **Default**: "--- Select Year ---" placeholder

#### Student Edit Form
- **Academic Year Dropdown**: Updated to "Year X" format
- **Location**: `onEditStudent()`, line ~1304
- **Pre-selection**: Uses `findData()` to correctly select current year

### 4. Admin Panel - Section Management

#### Section Creation Form
- **Academic Year Dropdown**: Updated to "Year X" format
- **Location**: `onAddSection()`, line ~386

#### Section Edit Form
- **Academic Year Dropdown**: Updated to "Year X" format
- **Location**: `onEditSection()`, line ~426-439
- **Major Fix**: Now fetches full Section object using `getSectionById()` to get accurate `academicLevelId`
- **Pre-selection**: Uses `findData()` instead of string comparison

#### Section Table Display
- **Academic Year Column**: Shows "Year X"
- **Location**: `refreshSectionsTable()`, line ~134
- **Fallback**: Legacy data shows "Year X (Legacy)" if using old semester_id field

## Bug Fixes Included

### Edit Form Pre-selection Issues (FIXED)
**Problem**: Edit forms were using `setCurrentIndex(count()-1)` which always selected the last item, not the current value.

**Solution**: Replaced with proper `findData()` lookup:
```cpp
// OLD (WRONG)
if (l.id() == c.yearLevel()) level->setCurrentIndex(level->count()-1);

// NEW (CORRECT)
int lvlIdx = level->findData(c.yearLevel());
if (lvlIdx >= 0) level->setCurrentIndex(lvlIdx);
```

**Affected Forms**:
- ✅ Course Edit Form
- ✅ Section Edit Form
- ✅ Student Edit Form (already correct)

### Section Edit Form Data Source (FIXED)
**Problem**: Section edit was reading display name from table instead of actual database ID.

**Solution**: Now fetches full Section object:
```cpp
Section sObj = m_sectionController.getSectionById(id);
levelIdx = level->findData(sObj.academicLevelId());
```

## Display Format Standard

All academic year selectors now follow this format:
- **Display Text**: "Year 1", "Year 2", "Year 3", "Year 4", "Year 5"
- **Stored Value**: Academic Level ID (from `academic_levels` table)
- **Sorting**: By `level_number` (ascending)

## Testing Checklist

- [ ] Professor Dashboard shows "Year 1-5" in year selector
- [ ] Creating a new course shows "Year X" options
- [ ] Editing a course pre-selects the correct year
- [ ] Creating a student shows "Year X" options
- [ ] Editing a student pre-selects the correct year
- [ ] Creating a section shows "Year X" options
- [ ] Editing a section pre-selects the correct year
- [ ] Course table "Level" column shows "Year X"
- [ ] Section table "Academic Year" column shows "Year X"

## Migration Notes

**No database changes required**. This is purely a UI/display update. The underlying data structure (`academic_levels` table with `id`, `name`, `level_number`) remains unchanged.

**Backward Compatibility**: The system still reads `level_number` from the database, so existing data works without modification.
