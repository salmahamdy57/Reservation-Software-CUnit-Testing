# Reservation-Software-CUnit-Testing

### Project Overview 📖
This repository contains an automated unit test suite for the Edges Academy Reservation system, implemented using the **CUnit** framework. The tests verify all core reservation workflows, account management, course enrollments, and login functionality for both Guests and Administrators.

### Objectives 🎯
- **Account Management**: Validate creating and deleting user accounts, including input validation for names, dates, and credentials.
- **Database Operations**: Test adding/deleting users under various conditions (full database, duplicate usernames, boundary IDs).
- **Login Flows**: Ensure correct detection of user roles (Admin vs. Normal), and verify admin login token logic with multiple attempts.
- **Course Registration**: Cover adding users to courses, preventing duplicate enrollments, handling full courses, and invalid IDs.
- **Course Deregistration**: Test deletion of course reservations, including edge cases (unenrolled students, non‐existent courses).
- **Database Reset**: Confirm `DBM_initDB()` correctly restores initial database state and default admin users.

### Key Tested Scenarios 🔍
1. **Create Account**
   - Valid data, min name/password lengths, oldest DOB year
   - Invalid DOB day, password mismatch, age-DOB consistency
2. **Add User to DB**
   - Valid addition, full database rejection, duplicate username handling
3. **Delete User from DB**
   - Valid deletion, non-existent IDs, middle-of-DB deletion without ID shifting, dependency cleanup
4. **Login Functionality**
   - `Detect_User_Type()` returns correct role
   - `Verify_Admin()` allows up to 3 attempts and enforces correct token
   - `Verify_User()` succeeds/fails based on credentials
5. **Course Enrollment**
   - `DBM_AddToCourse()` and `AddStudentToCourse()`: valid enrollments, capacity limits, invalid IDs
6. **Course Deregistration**
   - `DBM_DeleteReservation()`: valid removal, unenrolled or non-existent cases
7. **Database Initialization**
   - `DBM_initDB()`: resets all users, enrollments, and default data as per spec
