# Reservation-Software-CUnit-Testing

### Project Overview 📖

This **Reservation System** is a C-based command-line application that manages user accounts, course reservations, and administrative operations. This repository focuses exclusively on **testing and validation**, leveraging the CUnit framework and industry-standard techniques to ensure robustness, correctness, and comprehensive coverage across all software components.

### Objectives 🎯

- **Account Management**: Validate creating and deleting user accounts, including stringent input validation for names, dates of birth, and credentials.  
- **Database Operations**: Test adding and deleting users under various conditions, including full-database scenarios, duplicate usernames, and boundary IDs.  
- **Login Flows**: Ensure correct detection of user roles (Admin vs. Student), verify admin token authentication logic with up to three attempts, and validate student credential checks.  
- **Course Registration**: Cover adding users to courses, preventing duplicate enrollments, enforcing capacity limits, and handling invalid course IDs.  
- **Course Deregistration**: Test removal of course reservations, including edge cases such as unenrolled students and non-existent courses.  
- **Database Reset**: Confirm `DBM_initDB()` properly restores the initial database state with default users and enrollments.

### Deliverables 📑

- **CUnit Test Suites** structured by component:  
  - Database Manager: CRUD operations, capacity constraints, dependency cleanup  
  - Account Creation & Deletion: input validation, boundary ID handling  
  - Course Registration & Reservation: enrollment logic, capacity enforcement  
  - Login & Security: role detection, token verification, credential validation  
  - CLI Runners: AdminRunner, CustomerRunner, HomePageRunner, Main_App_Runner workflows  

- **Test Reports** including for each test case:  
  - Conditions, inputs, steps, expected vs. actual outputs  
  - Testing technique applied (MC/DC, boundary value analysis, state transition)  
  - Bug reports with reproduction steps, root causes, and suggested fixes  

- **Automation Harness**:  
  - `Main_Test_Runner()` initializes the CUnit registry, registers suites, and executes all tests via console interactive mode  

### Key Tested Scenarios 🔍

1. **Create Account**  
   - Valid data entry, minimum name/password lengths, oldest DOB year  
   - Invalid DOB day, password mismatch, age–DOB consistency checks  

2. **Add User to Database**  
   - Valid additions, full-database rejection, duplicate‐username handling  

3. **Delete User from Database**  
   - Valid deletion, non‐existent IDs, deletion in middle without ID shifting, cleanup of course dependencies  

4. **Login Functionality**  
   - `Detect_User_Type()`: correct role classification  
   - `Verify_Admin()`: allows up to 3 token attempts, enforces correct admin token  
   - `Verify_User()`: success/failure based on username/password validity and existence  

5. **Course Enrollment**  
   - `DBM_AddToCourse()` & `AddStudentToCourse()`: valid enrollments, capacity limits, duplicate and invalid ID checks  

6. **Course Deregistration**  
   - `DBM_DeleteReservation()`: valid removal, unenrolled or non‐existent inputs result in no‐ops  

7. **Database Initialization**  
   - `DBM_initDB()`: resets all users, default admin accounts, and sample enrollments as specified  

8. **CLI Workflow Runners**  
   - **AdminRunner**: add/delete users, list users, check reservations, help menu, invalid input handling  
   - **CustomerRunner**: account creation, multi‐step login/logout with retry logic, password change, user info display  
   - **HomePageRunner**: course reservation flows, enrolled courses display, logout path  
   - **Main_App_Runner**: entry branching for Admin vs. Student vs. invalid selections  
