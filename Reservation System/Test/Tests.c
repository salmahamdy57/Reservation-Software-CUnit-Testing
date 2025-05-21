#include "Tests.h"

/**************************************************************** Test Users For Test Cases ******************************************************************/
/* Valid Test User 1 */
static struct User_Input_Type Test1_User =
{
	{
	-1, // this is to indicate user hasn't assigned ID 
	"Salma",
	25,
	5,
	7,
	1999,
	Female,
	Graduate
	},
	{
		{
			"EdgesAcademy",
			"Edges123"
		},
	"Edges123"
	}
};

/* Valid Test User 2 */
static struct User_Input_Type Test2_User =
{
	{
	-1, // this is to indicate user hasn't assigned ID 
	"Khalid",
	18,
	11,
	10,
	2006,
	Male,
	Student
	},
	{
		{
			"EdgesAcademy2",
			"Edges1234"
		},
	"Edges1234"
	}
};

/* Invalid Test User */
static struct User_Input_Type Test_Invalid_User =
{
	{
	-1, // this is to indicate user hasn't assigned ID 
	"Sara",
	-1, //invalid age
	3,
	9,
	2001,
	Female,
	Student
	},
	{
		{
			"EdgesAcademy3",
			"Edges12345"
		},
	"Edges12345"
	}
};

/*************************************************************** Helper Functions For Testing ****************************************************************/
/* Centralized Cleanup Function */
static unsigned char universal_cleanup(void)
{	
	// Clear the database
	while (current_user_test > 0)
	{
		int user_id_to_delete = current_user_test - 1;
		DBM_Delete_User(user_id_to_delete); // Delete user without printing statements
	}

	// Reset user count
	current_user_test = 0;

	// Initialize database
	DBM_initDB();

	// Validate restoration
	CU_ASSERT_EQUAL(current_user_test, 3); // Should match initial number of users

	return 0;
}

/* Function to simulate input to bypass scanf and automate test cases */
void simulate_input(const char* input) 
{
	// Create a temporary file to simulate input
	FILE* temp_input = fopen("temp_input.txt", "w");
	if (temp_input == NULL) {
		perror("Error creating temporary file");
		exit(1);
	}
	// Simulated input
	fprintf(temp_input, "%s", input);
	fclose(temp_input);

	// Redirect stdin to the temporary input file
	freopen("temp_input.txt", "r", stdin);
}

/* Function to cleanup simulated input */
void cleanup_input() 
{
	// Restore original stdin
	freopen("CON", "r", stdin); // Use "/dev/tty" for Linux/macOS
	// Cleanup the temporary file
	remove("temp_input.txt");
}

// Buffer to store the output
static char output_buffer[1024];  

/* Function to redirect printf output to a buffer */
void redirect_printf_output(void) {
	setbuf(stdout, output_buffer);  // Redirect stdout to the buffer
}

/* Function to get the captured output */
const char* get_output(void) {
	return output_buffer;
}

/* Function to clear the output buffer */
void clear_output(void) {
	memset(output_buffer, 0, sizeof(output_buffer));
}

/* Function to check if a specific substring is present in the captured output */
void assert_printf_output_contains(const char* expected_substring) {
	const char* output = get_output();
	CU_ASSERT_TRUE(strstr(output, expected_substring) != NULL); // Check if substring exists in the output
}

/* Function to cleanup printf redirection */
void cleanup_printf_output(void) {
	setbuf(stdout, NULL);  // Reset stdout to default
	clear_output();        // Clear the output buffer
}

/********************************************************* Start and End Functions for Test Suites **********************************************************/
static unsigned char Start_DB_Manager_Tests(void) 
{ 
	printf("\nStart DBM Add User Tests... Current users: %d\n", current_user_test);
	//DBM_PrintUsers();
	//DBM_CheckReservations();
	return 0; 
}
static unsigned char End_DB_Manager_Tests(void) 
{ 
	universal_cleanup();
	printf("\nDatabase restored to initial state. Current users: %d\n", current_user_test);
	//DBM_PrintUsers();
	//DBM_CheckReservations();
	printf("\nEnd DBM Add User Tests...\n");
	return 0;
}

static unsigned char Start_Create_Account_Tests(void)
{
	printf("\nStart Create Account Tests... Current users: %d\n", current_user_test);
	//DBM_PrintUsers();
	return 0;
}
static unsigned char End_Create_Account_Tests(void)
{
	universal_cleanup();
	printf("\nDatabase restored to initial state. Current users: %d\n", current_user_test);
	//DBM_PrintUsers();
	printf("\nEnd Create Account Tests...\n");
	return 0;
}

static unsigned char Start_Course_Registration_Tests(void)
{
	printf("\nStart Course Registration Tests... Current users: %d\n", current_user_test);
	//DBM_PrintUsers();
	return 0;
}
static unsigned char End_Course_Registration_Tests(void)
{
	universal_cleanup();
	printf("\nDatabase restored to initial state. Current users: %d\n", current_user_test);
	//DBM_PrintUsers();
	printf("\nEnd Course Registration Tests...\n");
	return 0;
}

static unsigned char Start_Login_Tests(void)
{
	printf("\nStart Login Tests...\n");
	return 0;
}
static unsigned char End_Login_Tests(void)
{
	printf("\nEnd Login Tests...\n");
	return 0;
}

static unsigned char Start_Backend_Tests(void)
{
	printf("\nStart Backend Tests...\n");
	return 0;
}
static unsigned char End_Backend_Tests(void)
{
	printf("\nEnd Backend Tests...\n");
	return 0;
}

static unsigned char Start_App_Tests(void)
{
	printf("\nStart App Tests...\n");
	return 0;
}
static unsigned char End_App_Tests(void)
{
	printf("\nEnd App Tests...\n");
	return 0;
}

/******************************************************************** Database Test Cases ********************************************************************/

/************************************************************************************
* Test ID                : DBM_AddUser_WithValidData
* Description            : Testing the Add Valid User Functionality
* Pre-requisites         : There is space in the database to add a new user
* Test inputs            :
*                          Name                         -> Salma
*                          Age                          -> 25
*                          DOB_day                      -> 5
*                          DOB_Month                    -> 7
*                          DOB_Year                     -> 1999
*                          Educational_Status           -> Graduate
*                          Gender                       -> Female
*                          UserName                     -> EdgesAcademy
*                          Password                     -> Edges123
*                          Password Recheck             -> Edges123
* Test Expected output   : The DB should be updated with the previous inputs correctly
* Reason                 : All inputs are valid, and the user should be successfully added to the database
*************************************************************************************/
static void DBM_AddUser_WithValidData(void)
{
	unsigned char RET = DBM_Add_User(&Test1_User);
	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1); 

	// Assign ID to test user
	Test1_User.PersonalInfo_Form.id = current_user_test - 1;

	unsigned int Test_id = Test1_User.PersonalInfo_Form.id;
	CU_ASSERT_EQUAL(DB_info[Test_id].Age, Test1_User.PersonalInfo_Form.Age);
	CU_ASSERT_EQUAL(DB_info[Test_id].DOB_day, Test1_User.PersonalInfo_Form.DOB_day);
	CU_ASSERT_EQUAL(DB_info[Test_id].DOB_month, Test1_User.PersonalInfo_Form.DOB_month);
	CU_ASSERT_EQUAL(DB_info[Test_id].DOB_year, Test1_User.PersonalInfo_Form.DOB_year);
	CU_ASSERT_EQUAL(DB_info[Test_id].educational_status, Test1_User.PersonalInfo_Form.educational_status);
	CU_ASSERT_EQUAL(DB_info[Test_id].gender, Test1_User.PersonalInfo_Form.gender);
	CU_ASSERT_STRING_EQUAL(DB_info[Test_id].name, Test1_User.PersonalInfo_Form.name);
	CU_ASSERT_STRING_EQUAL(DB_LoginCredentials[Test_id].User_Name, Test1_User.LoginCredentials_Form.LoginCredentials.User_Name);
	CU_ASSERT_STRING_EQUAL(DB_LoginCredentials[Test_id].Password, Test1_User.LoginCredentials_Form.LoginCredentials.Password);
}

/************************************************************************************
* Test ID                : DBM_AddUser_WithInvalidData
* Description            : Testing the Add Invalid User Functionality
* Pre-requisites         : Database is already initialized
* Test inputs            :
*                          Name                         -> " "
*                          Age                          -> -5 (Invalid)
*                          DOB_day                      -> 32 (Invalid)
*                          DOB_Month                    -> 7
*                          DOB_Year                     -> 1999
*                          Educational_Status           -> Graduate
*                          Gender                       -> Female
*                          UserName                     -> " "
*                          Password                     -> Edges123
*                          Password Recheck             -> Edges123
* Test Expected output   : The DB should reject the user and remain unchanged
* Reason                 : Invalid inputs (negative age, invalid date of birth, empty name or login) should prevent user addition
*************************************************************************************/
static void DBM_AddUser_WithInvalidData(void)
{
	// Capture the initial user count
	unsigned int initial_user_count = current_user_test;

	// Scenario 1: Invalid age
	struct User_Input_Type invalid_user_age = Test1_User;
	invalid_user_age.PersonalInfo_Form.Age = -5; // Negative age
	CU_ASSERT_EQUAL(DBM_Add_User(&invalid_user_age), 0);

	// Scenario 2: Invalid date of birth
	struct User_Input_Type invalid_user_dob = Test1_User;
	invalid_user_dob.PersonalInfo_Form.DOB_day = 32; // Invalid day
	CU_ASSERT_EQUAL(DBM_Add_User(&invalid_user_dob), 0);

	// Scenario 3: Empty name
	struct User_Input_Type invalid_user_name = Test1_User;
	strcpy(invalid_user_name.PersonalInfo_Form.name, ""); // Empty name
	CU_ASSERT_EQUAL(DBM_Add_User(&invalid_user_name), 0);

	// Scenario 4: Empty login credentials
	struct User_Input_Type invalid_user_credentials = Test1_User;
	strcpy(invalid_user_credentials.LoginCredentials_Form.LoginCredentials.User_Name, "");
	CU_ASSERT_EQUAL(DBM_Add_User(&invalid_user_credentials), 0);

	CU_ASSERT_EQUAL(initial_user_count, current_user_test);

	//return DB to its initial state
	universal_cleanup();
}

/************************************************************************************
* Test ID                : DBM_AddUser_WhenDatabaseIsFull
* Description            : Testing the Add User When Database Is Full
* Pre-requisites         : Database should be filled to its maximum capacity
* Test inputs            : User details similar to "Salma" but different credentials
* Test Expected output   : The database should reject adding a new user when it’s full
* Reason                 : The test ensures that no new users can be added when the database reaches its capacity
*************************************************************************************/
static void DBM_AddUser_WhenDatabaseIsFull(void)
{
	// Fill the database to its maximum capacity
	for (int i = current_user_test; i < MAX_USERS; i++) {
		struct User_Input_Type temp_user = Test1_User;
		snprintf(temp_user.PersonalInfo_Form.name, sizeof(temp_user.PersonalInfo_Form.name), "User%d", i);
		snprintf(temp_user.LoginCredentials_Form.LoginCredentials.User_Name, sizeof(temp_user.LoginCredentials_Form.LoginCredentials.User_Name), "Username%d", i);
		CU_ASSERT_EQUAL(DBM_Add_User(&temp_user), 1);
	}

	// Validate database is full
	CU_ASSERT_EQUAL(current_user_test, MAX_USERS);

	// Attempt to add another user
	unsigned char RET = DBM_Add_User(&Test2_User);
	CU_ASSERT_EQUAL(RET, 0); // Ensure addition fails
	CU_ASSERT_EQUAL(current_user_test, MAX_USERS);

	//return DB to its initial state
	universal_cleanup();
}

/************************************************************************************
* Test ID                : DBM_AddUser_WithDuplicateUsername
* Description            : Testing the Add User with a duplicate username that already exists in DB
* Pre-requisites         : Database should be initialized
* Test inputs            : Test User 2 data but same user name as Test user 1 "EdgesAcademy"
* Test Expected output   : The database should reject adding a new user with an existing username
* Reason                 : The test ensures that only unique usernames should be accepted
*************************************************************************************/
static void DBM_AddUser_WithDuplicateUsername(void)
{
	// Add test user to DB
	unsigned char RET = DBM_Add_User(&Test1_User);
	CU_ASSERT_EQUAL(RET, 1); // Validate addition success

	// Assign ID to test user
	Test1_User.PersonalInfo_Form.id = current_user_test - 1;

	// Create a new user with a duplicate username
	struct User_Input_Type duplicate_username = Test2_User;
	strcpy(duplicate_username.LoginCredentials_Form.LoginCredentials.User_Name, "EdgesAcademy"); // Duplicate username

	// Validate addition is rejected
	CU_ASSERT_EQUAL(DBM_Add_User(&duplicate_username), 0);

	// Ensure user count remains unchanged
	CU_ASSERT_EQUAL(current_user_test, Test1_User.PersonalInfo_Form.id + 1);

	// Validate that no new user was added to the database
	for (unsigned int i = 0; i < current_user_test; i++) {
		CU_ASSERT_STRING_NOT_EQUAL(DB_info[i].name, duplicate_username.PersonalInfo_Form.name);
	}
}

/************************************************************************************
* Test ID                : DBM_DeleteUser_WithValidID
* Description            : Testing the Delete Valid User Functionality
* Pre-requisites         : Test 1 User valid ID = current_user_test - 1
* Test inputs            : User ID to delete -> ID of the user to delete (e.g., "Salma")
* Test Expected output   : The user should be successfully deleted, and the database count should decrease
* Reason                 : Deleting a valid user should update the database correctly
*************************************************************************************/
static void DBM_DeleteUser_WithValidID(void)
{
	// Add test user to DB
	unsigned char RET = DBM_Add_User(&Test1_User);
	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1); 

	// Assign ID to test user
	Test1_User.PersonalInfo_Form.id = current_user_test - 1;
	// Capture the test user's ID
	unsigned int test_user_id = Test1_User.PersonalInfo_Form.id;

	// Record the user count after test user addition
	unsigned int initial_user_count = current_user_test;

	// Attempt to delete the test user
	unsigned char result = DBM_Delete_User(test_user_id);

	// Validate deletion success
	CU_ASSERT_EQUAL(result, 1);

	// Validate user count is decremented
	CU_ASSERT_EQUAL(current_user_test, initial_user_count - 1);

	// Validate that the test user is no longer in the database
	CU_ASSERT_STRING_NOT_EQUAL(DB_info[test_user_id].name, Test1_User.PersonalInfo_Form.name);
	
	//return DB to its initial state
	universal_cleanup();
}

/************************************************************************************
* Test ID                : DBM_DeleteUser_WithNonExistentID
* Description            : Testing the Delete Nonexistent User Functionality
* Pre-requisites         : Database contains users
* Test inputs            : Nonexistent User ID (e.g., ID greater than the current maximum ID)
* Test Expected output   : The deletion attempt should fail, and the user count should remain the same
* Reason                 : Attempting to delete a non-existent user should not modify the database
*************************************************************************************/
static void DBM_DeleteUser_WithNonExistentID(void)
{
	// Capture the initial user count
	unsigned int initial_user_count = current_user_test;

	// Generate a nonexistent user ID
	unsigned int nonexistent_user_id = current_user_test + 1; // ID outside the valid range

	// Attempt to delete the nonexistent user ID
	unsigned char result = DBM_Delete_User(nonexistent_user_id);

	// Validate that the function rejected the operation
	CU_ASSERT_EQUAL(result, 0);

	// Validate that the user count is unchanged
	CU_ASSERT_EQUAL(initial_user_count, current_user_test);

	//return DB to its initial state
	universal_cleanup();
}
/************************************************************************************
* Test ID                : DBM_DeleteUser_FromMiddleOfDatabase
* Description            : Testing Deletion of a User from the Middle of the Database
* Pre-requisites         : The database is initialized with multiple users
* Test inputs            : User ID to delete -> A user in the middle of the database (e.g., ID 1)
* Test Expected output   : The user should be deleted, and the IDs of remaining users should not change (no shifting of IDs)
* Reason                 : Ensure deletion from the middle of the database does not alter the IDs of other users
*************************************************************************************/
static void DBM_DeleteUser_FromMiddleOfDatabase(void)
{
	// Capture the initial user count
	int initial_user_count = current_user_test;

	// Delete middle user (e.g., user 1)
	int middle_user_id = 1;
	CU_ASSERT_EQUAL(DBM_Delete_User(middle_user_id), 1);

	// Verify the user count is updated correctly
	CU_ASSERT_EQUAL(current_user_test, initial_user_count - 1);  // Decremented by 1

	// Validate that the remaining users' IDs are still consistent (no shift)
	int user_index = 0;
	for (int i = 0; i < initial_user_count; i++) {
		if (i != middle_user_id) {
			CU_ASSERT_EQUAL(DB_info[user_index].id, i);
			user_index++;  // Only increment for users that were not deleted
		}
	}
	//return DB to its initial state
	universal_cleanup();
}
/************************************************************************************
* Test ID                : DBM_DeleteUser_WithDependencies
* Description            : Testing Deletion of a User with Existing Course Enrollments
* Pre-requisites         : Database tracks user enrollments using DBM_AddToCourse()
* Test inputs            : User ID to delete -> ID of the user with enrollments
*                          Courses -> Set of courses the user is enrolled in
* Test Expected output   : - The user should be successfully deleted.                         
*                          - Enrollments[user_id][*] should be cleared.
* Reason                 : Ensures that deletion logic properly handles associated course enrollments.
*************************************************************************************/
static void DBM_DeleteUser_WithDependencies(void)
{
	//printf("\nuser with id 0 courses:\n");
	//DBM_ShowCourse(0);
	//printf("\nuser with id 1 courses:\n");
	//DBM_ShowCourse(1);
	//printf("\nuser with id 2 courses:\n");
	//DBM_ShowCourse(2);
	//DBM_CheckReservations();
	// Capture the initial user count
	unsigned int initial_user_count = current_user_test;

	// Add a test user to the database
	unsigned char RET = DBM_Add_User(&Test1_User);
	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1); 

	// Assign ID to the test user
	Test1_User.PersonalInfo_Form.id = current_user_test - 1;
	unsigned int test_user_id = Test1_User.PersonalInfo_Form.id;

	//printf("\nTest user added with id: %d\n", test_user_id);
	//DBM_PrintUsers();

	// Enroll the user in courses using DBM_AddToCourse()
	int course_1 = 1, course_2 = 2; // Example course IDs
	CU_ASSERT_EQUAL(DBM_AddToCourse(course_1, test_user_id), 1); // Enroll test user in course 1
	CU_ASSERT_EQUAL(DBM_AddToCourse(course_2, test_user_id), 1); // Enroll test user in course 2

	// Verify enrollments are set
	CU_ASSERT_EQUAL(Enrollments[test_user_id][course_1 - 1], 1);
	CU_ASSERT_EQUAL(Enrollments[test_user_id][course_2 - 1], 1);

	// Display user's enrolled courses and overall reservations before deletion
	//printf("\ntest user courses:\n");
	//DBM_ShowCourse(test_user_id);
	//DBM_CheckReservations();

	// Delete the user
	unsigned char delete_result = DBM_Delete_User(test_user_id);
	CU_ASSERT_EQUAL(delete_result, 1); // Validate deletion success

	// Validate enrollments are cleared
	CU_ASSERT_EQUAL(Enrollments[test_user_id][course_1 - 1], 0);
	CU_ASSERT_EQUAL(Enrollments[test_user_id][course_2 - 1], 0);

	// Validate the user count returns to initial count
	CU_ASSERT_EQUAL(current_user_test, initial_user_count);

	// Display user's enrolled courses and overall reservations after deletion
	//printf("\nAfter deleting user:\n");
	//DBM_PrintUsers();

	//printf("\ntest user courses:\n");
	//DBM_ShowCourse(test_user_id);
	//DBM_CheckReservations();

	//DBM_PrintUsers(); // Optional: Validate the database state for debugging

	//return DB to its initial state
	universal_cleanup();

}
/************************************************************************************
* Test ID                : DBM_DeleteUser_FromEmptyDatabase
* Description            : Testing Deletion of a User from an Empty Database
* Pre-requisites         : Database is empty
* Test inputs            : User ID to delete -> Any ID (attempting deletion when DB is empty)
* Test Expected output   : The deletion attempt should fail as the database is empty
* Reason                 : Deleting a user from an empty database should result in failure and no change to the database
*************************************************************************************/
static void DBM_DeleteUser_FromEmptyDatabase(void)
{
	// Clear database
	while (current_user_test > 0) {
		int user_id_to_delete = current_user_test - 1;
		CU_ASSERT_EQUAL(DBM_Delete_User(user_id_to_delete), 1);
	}
	// Validate database is empty
	CU_ASSERT_EQUAL(current_user_test, 0);

	// Try deleting a user from an empty DB
	unsigned char empty_delete_result = DBM_Delete_User(0); // Attempt to delete from an empty DB
	CU_ASSERT_EQUAL(empty_delete_result, 0); // Ensure the function rejects the operation

	//DBM_PrintUsers(); //should print empty DB
	
	// Initialize DB
	DBM_initDB();
	//printf("\nre-initialized db\n");
	//DBM_PrintUsers(); //should print initialized DB
}

/************************************************************************************
* Test ID                : DBM_initDB_WithValidData
* Description            : Testing the DBM_initDB function to ensure the database
*                          is properly initialized or reset.
* Pre-requisites         : The database should contain some users before initialization.
* Test inputs            : None (the test will check the database state after calling DBM_initDB).
* Test Expected output   : The database should be reset to its initial state,
*                          and the user count should match the expected initial value.
* Reason                 : Ensures that DBM_initDB resets the database as expected.
*************************************************************************************/
static void DBM_initDB_WithValidData(void)
{
	// Capture the initial user count
	unsigned int initial_user_count = current_user_test;

	// Add a test user to modify the database state
	struct User_Input_Type test_user = Test1_User;
	unsigned char RET = DBM_Add_User(&test_user);
	CU_ASSERT_EQUAL(RET, 1); // Validate addition success

	// Ensure the user count has increased
	unsigned int modified_user_count = current_user_test;
	CU_ASSERT(modified_user_count > initial_user_count); // Should be greater than initial count

	// Call DBM_initDB to reset the database
	DBM_initDB();

	// Validate that the database is reset to its initial state
	CU_ASSERT_EQUAL(current_user_test, initial_user_count); // Should match initial count

	// Validate default user data
	CU_ASSERT_STRING_EQUAL(DB_info[0].name, "Ahmed");
	CU_ASSERT_EQUAL(DB_info[0].Age, 23);
	CU_ASSERT_EQUAL(DB_info[0].DOB_day, 1);
	CU_ASSERT_EQUAL(DB_info[0].DOB_month, 2);
	CU_ASSERT_EQUAL(DB_info[0].DOB_year, 2021);
	CU_ASSERT_STRING_EQUAL(DB_LoginCredentials[0].User_Name, "AdminUser1");
	CU_ASSERT_STRING_EQUAL(DB_LoginCredentials[0].Password, "Edges123");

	CU_ASSERT_STRING_EQUAL(DB_info[1].name, "Mohamed");
	CU_ASSERT_EQUAL(DB_info[1].Age, 25);
	CU_ASSERT_EQUAL(DB_info[1].DOB_day, 5);
	CU_ASSERT_EQUAL(DB_info[1].DOB_month, 11);
	CU_ASSERT_EQUAL(DB_info[1].DOB_year, 1999);
	CU_ASSERT_STRING_EQUAL(DB_LoginCredentials[1].User_Name, "AdminUser2");
	CU_ASSERT_STRING_EQUAL(DB_LoginCredentials[1].Password, "Edges123456");

	CU_ASSERT_STRING_EQUAL(DB_info[2].name, "Nada");
	CU_ASSERT_EQUAL(DB_info[2].Age, 24);
	CU_ASSERT_EQUAL(DB_info[2].DOB_day, 3);
	CU_ASSERT_EQUAL(DB_info[2].DOB_month, 4);
	CU_ASSERT_EQUAL(DB_info[2].DOB_year, 2000);
	CU_ASSERT_STRING_EQUAL(DB_LoginCredentials[2].User_Name, "AdminUser3");
	CU_ASSERT_STRING_EQUAL(DB_LoginCredentials[2].Password, "Edges_123");

	// Validate course enrollments
	CU_ASSERT_EQUAL(Enrollments[0][4], 1); // User 0 enrolled in Course 5
	CU_ASSERT_EQUAL(Enrollments[0][5], 1); // User 0 enrolled in Course 6

	CU_ASSERT_EQUAL(Enrollments[1][4], 1); // User 1 enrolled in Course 5
	CU_ASSERT_EQUAL(Enrollments[1][5], 1); // User 1 enrolled in Course 6

	CU_ASSERT_EQUAL(Enrollments[2][4], 1); // User 2 enrolled in Course 5
	CU_ASSERT_EQUAL(Enrollments[2][0], 1); // User 2 enrolled in Course 1

}

/************************************************************************************
* Test ID                : DBM_AddToCourse_WithValidData
* Description            : Tests adding a user to a valid course.
* Pre-requisites         : The user exists in the database, and the course exists.
* Test inputs            : CourseID            -> 1
*                          Test UserID         -> current_user_test - 1
* Test Expected output   : The user should be successfully added to the course, and the enrollment should be reflected in the Enrollments array.
* Reason                 : All inputs are valid, the course should be successfully added to the user
*************************************************************************************/
static void DBM_AddToCourse_WithValidData(void)
{
	// Add a test user to the database
	unsigned char RET = DBM_Add_User(&Test1_User);
	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1);

	// Assign ID to the test user
	Test1_User.PersonalInfo_Form.id = current_user_test - 1;
	unsigned int test_user_id = Test1_User.PersonalInfo_Form.id;

	// Enroll the user in courses 
	// Example course IDs
	int course_1 = 1;
	// Add course to test user
	unsigned char result = DBM_AddToCourse(course_1, test_user_id);

	// Enrollment should be successful
	CU_ASSERT_EQUAL(result, 1); // Enroll test user in course 1
	// Enrollment should be recorded
	CU_ASSERT_EQUAL(Enrollments[test_user_id][course_1 - 1], 1);

	//return DB to its initial state
	universal_cleanup();
}

/************************************************************************************
* Test ID                : DBM_AddToCourse_NonExistentCourse
* Description            : Tests the behavior of adding a user to a non-existent course.
* Pre-requisites         : The user exists in the database.
* Test inputs            : CourseID            -> 10 
*                          Test UserID         -> current_user_test - 1
* Test Expected output   : The enrollment should fail, and the Enrollments table should not reflect the user’s enrollment in the non-existent course.
* Reason                 : The system should not allow users to be enrolled in courses that don't exist.
*************************************************************************************/
static void DBM_AddToCourse_NonExistentCourse(void)
{
	// Add a test user to the database
	unsigned char RET = DBM_Add_User(&Test1_User);
	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1);

	// Assign ID to the test user
	Test1_User.PersonalInfo_Form.id = current_user_test - 1;
	unsigned int test_user_id = Test1_User.PersonalInfo_Form.id;

	// Attempt to enroll the user in a nonexistent courses 
	int course = 10;
	// Add course to test user
	unsigned char result = DBM_AddToCourse(course, test_user_id);

	// Enrollment should fail
	CU_ASSERT_EQUAL(result, 0); 
	// Enrollment should be rejected
	CU_ASSERT_EQUAL(Enrollments[test_user_id][course - 1], 0);

	//return DB to its initial state
	universal_cleanup();
}

/************************************************************************************
* Test ID                : DBM_AddToCourse_AlreadyEnrolled
* Description            : Tests adding a user to a course they are already enrolled in.
* Pre-requisites         : The user is already enrolled in the course.
* Test inputs            : CourseID            -> 1 (enroll test user twice)
*                          Test UserID         -> current_user_test - 1
* Test Expected output   : The system should not allow double enrollment, and the Enrollments array should reflect the user's enrollment without change.
* Reason                 : Prevent double enrollment of users in the same course.
*************************************************************************************/
static void DBM_AddToCourse_AlreadyEnrolled(void)
{
	// Add a test user to the database
	unsigned char RET = DBM_Add_User(&Test1_User);
	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1);

	// Assign ID to the test user
	Test1_User.PersonalInfo_Form.id = current_user_test - 1;
	unsigned int test_user_id = Test1_User.PersonalInfo_Form.id;

	// Enroll user in course
	int course = 1;
	// Add course to test user
	unsigned char result_1 = DBM_AddToCourse(course, test_user_id);

	// Enrollment should be successful
	CU_ASSERT_EQUAL(result_1, 1);
	// Enrollment should be recorded
	CU_ASSERT_EQUAL(Enrollments[test_user_id][course - 1], 1);

	// Attempt to add the same user to the same course again
	unsigned char result_2 = DBM_AddToCourse(course, test_user_id);

	// Enrollment should fail
	CU_ASSERT_EQUAL(result_2, 0);
	// Enrollment should still be recorded (status remains 1)
	CU_ASSERT_EQUAL(Enrollments[test_user_id][course - 1], 1);

	//return DB to its initial state
	universal_cleanup();
}

/************************************************************************************
* Test ID                : DBM_AddToCourse_FullCourse
* Description            : Tests adding a user to a course that is full.
* Pre-requisites         : The course exists and is full.
* Test inputs            : A valid Course_ID and User_ID, where the course is full.
* Test Expected output   : The system should reject the enrollment and not add the user to the course.
* Reason                 : The system should prevent users from enrolling in a course that has reached its capacity.
*************************************************************************************/
static void DBM_AddToCourse_FullCourse(void)
{
	// Add users to the database
	unsigned char RET = DBM_Add_User(&Test1_User);
	CU_ASSERT_EQUAL(RET, 1);

	// Assign ID to the test user
	Test1_User.PersonalInfo_Form.id = current_user_test - 1;
	unsigned int test_user_id = Test1_User.PersonalInfo_Form.id;

	// Enroll users in the course until it is full
	int course_id = 2; 
	unsigned char result;

	// First, enroll users to fill the course
	for (int i = 0; i < MAX_STUDENTS_PER_COURSE; i++) {
		result = DBM_AddToCourse(course_id, i);  // Enroll a user with index i
		CU_ASSERT_EQUAL(result, 1);               // Ensure that the enrollment was successful
	}

	// Now, try to enroll the next user (which should fail as the course is full)
	result = DBM_AddToCourse(course_id, test_user_id);

	// The enrollment should fail
	CU_ASSERT_EQUAL(result, 0);
	
	int current_enrollment_count = 0;
	for (int i = 0; i < MAX_USERS; i++) {
		if (Enrollments[i][course_id - 1] == TRUE) {
			current_enrollment_count++;
		}
	}
	// Ensure no overflow
	CU_ASSERT_EQUAL(current_enrollment_count, MAX_STUDENTS_PER_COURSE);  
}

/************************************************************************************
* Test ID                : DBM_DeleteReservation_WithValidData
* Description            : Tests deleting a reservation for a valid student in a valid course.
* Pre-requisites         : The student is enrolled in the course.
* Test inputs            : CourseID            -> 1
*                          Test UserID         -> current_user_test - 1
* Test Expected output   : The student’s reservation should be deleted, and the Enrollments array should no longer reflect the student’s enrollment in the course.
* Reason                 : The system should correctly handle removing a reservation.
*************************************************************************************/
static void DBM_DeleteReservation_WithValidData(void)
{
	// Add a test user to the database
	unsigned char RET = DBM_Add_User(&Test1_User);
	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1);

	// Assign ID to the test user
	Test1_User.PersonalInfo_Form.id = current_user_test - 1;
	unsigned int test_user_id = Test1_User.PersonalInfo_Form.id;

	// Enroll the user in courses 
	// Example course IDs
	int course_1 = 1;
	// Add course to test user
	unsigned char result_add = DBM_AddToCourse(course_1, test_user_id);

	// Enrollment should be successful
	CU_ASSERT_EQUAL(result_add, 1);
	// Enrollment should be recorded
	CU_ASSERT_EQUAL(Enrollments[test_user_id][course_1 - 1], 1);

	// Delete course reservation from test user
	unsigned char result_delete = DBM_DeleteReservation(course_1, test_user_id);

	// Deletion should be successful
	CU_ASSERT_EQUAL(result_delete, 1); 
	// Validate enrollments are removed
	CU_ASSERT_EQUAL(Enrollments[test_user_id][course_1 - 1], 0);

	//return DB to its initial state
	universal_cleanup();
}

/************************************************************************************
* Test ID                : DBM_DeleteReservation_StudentNotEnrolled
* Description            : Tests deleting a reservation for a student not enrolled in the course.
* Pre-requisites         : The student is not enrolled in the course.
* Test inputs            : CourseID            -> 1
*                          Test UserID         -> current_user_test - 1 (not enrolled)
* Test Expected output   : The deletion attempt should fail, and the Enrollments table should not be affected.
* Reason                 : The system should not allow deleting reservations for non-enrolled students.
*************************************************************************************/
static void DBM_DeleteReservation_StudentNotEnrolled(void)
{
	// Add a test user to the database
	unsigned char RET = DBM_Add_User(&Test1_User);
	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1);

	// Assign ID to the test user
	Test1_User.PersonalInfo_Form.id = current_user_test - 1;
	unsigned int test_user_id = Test1_User.PersonalInfo_Form.id;

	// Example course ID
	int course_1 = 1;

	// Attempt to delete course reservation from unenrolled test user
	unsigned char result = DBM_DeleteReservation(course_1, test_user_id);

	// Deletion should be rejected
	CU_ASSERT_EQUAL(result, 0); 
	// Validate no enrollments allowed
	CU_ASSERT_EQUAL(Enrollments[test_user_id][course_1 - 1], 0);

	//return DB to its initial state
	universal_cleanup();
}

/************************************************************************************
* Test ID                : DBM_DeleteReservation_NonExistentCourse
* Description            : Tests deleting a reservation for a non-existent course.
* Pre-requisites         : The student exists, but the course does not exist.
* Test inputs            : CourseID            -> 10 (doesn't exist)
*                          Test UserID         -> current_user_test - 1 
* Test Expected output   : The deletion should fail, and the Enrollments table should remain unchanged.
* Reason                 : The system should not allow deleting reservations for non-existent courses.
*************************************************************************************/
static void DBM_DeleteReservation_NonExistentCourse(void)
{
	// Add a test user to the database
	unsigned char RET = DBM_Add_User(&Test1_User);
	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1);

	// Assign ID to the test user
	Test1_User.PersonalInfo_Form.id = current_user_test - 1;
	unsigned int test_user_id = Test1_User.PersonalInfo_Form.id;

	// Example course ID
	int course_1 = 100;

	// Attempt to delete nonexistent course reservation
	unsigned char result = DBM_DeleteReservation(course_1, test_user_id);

	// Deletion should be rejected
	CU_ASSERT_EQUAL(result, 0);
	// Validate no enrollments allowed
	CU_ASSERT_EQUAL(Enrollments[test_user_id][course_1 - 1], 0);

	//return DB to its initial state
	universal_cleanup();
}

/************************************************************************************
* Test ID                : DBM_DeleteReservation_InvalidStudentID
* Description            : Tests deleting a reservation using an invalid student ID.
* Pre-requisites         : The course exists, but the student ID provided does not exist in the system.
* Test inputs            : CourseID            -> 1 
*                          Test UserID         -> invalid_user_id (not in the system)
* Test Expected output   : The deletion should fail, and the Enrollments table should remain unchanged.
* Reason                 : The system should not allow deleting reservations for non-existent students.
*************************************************************************************/
static void DBM_DeleteReservation_InvalidStudentID(void)
{
	// Example course ID
	int course_1 = 1;

	// Assign a test user ID that does not exist in the database (invalid ID)
	unsigned int invalid_user_id = 20; 

	// Attempt to delete reservation for a non-existent student
	unsigned char result = DBM_DeleteReservation(course_1, invalid_user_id);

	// Deletion should be rejected (since student doesn't exist)
	CU_ASSERT_EQUAL(result, 0);
	// No change to enrollment for non-existent student
	CU_ASSERT_EQUAL(Enrollments[invalid_user_id][course_1 - 1], 0); // Should remain 0 for invalid user ID

	// Return DB to its initial state
	universal_cleanup();
}

/************************************************************************************
* Test ID                : DBM_DeleteReservation_NoEnrollments
* Description            : Tests deleting a reservation when the course has no enrollments.
* Pre-requisites         : The course exists but has no enrolled students.
* Test inputs            : CourseID            -> 2 (no enrolled students)
*                          Test UserID         -> current_user_test - 1
* Test Expected output   : The deletion should fail, and the Enrollments table should remain unchanged.
* Reason                 : The system should not't allow deleting reservations for courses with no enrolled students.
*************************************************************************************/
static void DBM_DeleteReservation_NoEnrollments(void)
{
	// Add a test user to the database
	unsigned char RET = DBM_Add_User(&Test1_User);
	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1);

	// Assign ID to the test user
	Test1_User.PersonalInfo_Form.id = current_user_test - 1;
	unsigned int test_user_id = Test1_User.PersonalInfo_Form.id;

	// Course ID with no enrollments
	int course_2 = 2;

	// Attempt to delete reservation for an empty course
	unsigned char result = DBM_DeleteReservation(course_2, test_user_id);

	// Deletion should fail
	CU_ASSERT_EQUAL(result, 0);
	// No change to enrollment
	CU_ASSERT_EQUAL(Enrollments[test_user_id][course_2 - 1], 0);
}

/******************************************************************** Create Account Test Cases ******************************************************************/

/************************************************************************************
* Test ID                : Create_Account_WithValidData
* Description            : Testing Create Account with Valid Data Functionality
* Pre-requisites         : There is space in the database to add a new account
* Test inputs            :
*                          Name                         -> Salma
*                          Age                          -> 25
*                          DOB_day                      -> 5
*                          DOB_Month                    -> 7
*                          DOB_Year                     -> 1999
*                          Educational_Status           -> Graduate
*                          Gender                       -> Female
*                          UserName                     -> EdgesAcademy
*                          Password                     -> Edges123
*                          Password Recheck             -> Edges123
* Test Expected output   : The DB should be updated with the previous inputs correctly
* Reason                 : All inputs are valid, and the account should be successfully added to the database
*************************************************************************************/
static void Add_Account_WithValidData(void)
{
	// Add test user account
	unsigned char RET = Add_Account(&Test1_User);
	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1);

	// Assign ID to test user
	Test1_User.PersonalInfo_Form.id = current_user_test - 1;

	unsigned int Test_id = Test1_User.PersonalInfo_Form.id;

	// Validate the DB was updated correctly with the user data
	CU_ASSERT_EQUAL(DB_info[Test_id].Age, Test1_User.PersonalInfo_Form.Age);
	CU_ASSERT_EQUAL(DB_info[Test_id].DOB_day, Test1_User.PersonalInfo_Form.DOB_day);
	CU_ASSERT_EQUAL(DB_info[Test_id].DOB_month, Test1_User.PersonalInfo_Form.DOB_month);
	CU_ASSERT_EQUAL(DB_info[Test_id].DOB_year, Test1_User.PersonalInfo_Form.DOB_year);
	CU_ASSERT_EQUAL(DB_info[Test_id].educational_status, Test1_User.PersonalInfo_Form.educational_status);
	CU_ASSERT_EQUAL(DB_info[Test_id].gender, Test1_User.PersonalInfo_Form.gender);
	CU_ASSERT_STRING_EQUAL(DB_info[Test_id].name, Test1_User.PersonalInfo_Form.name);
	CU_ASSERT_STRING_EQUAL(DB_LoginCredentials[Test_id].User_Name, Test1_User.LoginCredentials_Form.LoginCredentials.User_Name);
	CU_ASSERT_STRING_EQUAL(DB_LoginCredentials[Test_id].Password, Test1_User.LoginCredentials_Form.LoginCredentials.Password);

	//return DB to its initial state
	universal_cleanup();
}

/************************************************************************************
* Test ID                : Create_Account_With_Invalid_DOB_Day
* Description            : Testing the Add Invalid User Functionality
* Pre-requisites         : Database is already initialized
* Test inputs            : Test user 1 with invalid DOB_day (31/02/1999)
* Test Expected output   : The DB should reject the user and remain unchanged
* Reason                 : Invalid inputs (February doesn't have 31 days) should prevent user addition
*************************************************************************************/
static void Add_Account_With_Invalid_DOB_Day(void)
{
	// Capture the initial user count
	unsigned int initial_user_count = current_user_test;

	// Invalid Date Of Birth
	struct User_Input_Type invalid_user_dob = Test1_User;
	invalid_user_dob.PersonalInfo_Form.DOB_day = 31;
	invalid_user_dob.PersonalInfo_Form.DOB_month = 2;
	// User should be rejected
	CU_ASSERT_EQUAL(Add_Account(&invalid_user_dob), 0);

	// User count should remain the same
	CU_ASSERT_EQUAL(initial_user_count, current_user_test);

	//return DB to its initial state
	universal_cleanup();
}

/************************************************************************************
* Test ID                : Create_Account_With_Oldest_DOB_Year
* Description            : Testing Add account Functionality
* Pre-requisites         : Database is already initialized
* Test inputs            : Test user 1 with DOB_year (1924)
* Test Expected output   : The DB should be updated with the previous inputs correctly
* Reason                 : All inputs are valid, and the account should be successfully added to the database
*************************************************************************************/
static void Add_Account_With_Oldest_DOB_Year(void)
{
	// Capture the initial user count
	unsigned int initial_user_count = current_user_test;

	// Valid DOB oldest year
	struct User_Input_Type user_dob_oldest_year = Test1_User;
	user_dob_oldest_year.PersonalInfo_Form.DOB_year = 1924;

    // Add test user account
	unsigned char RET = Add_Account(&Test1_User);
	// Assign ID to test user
	Test1_User.PersonalInfo_Form.id = current_user_test - 1;

	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1);

	// User count should be incremented
	CU_ASSERT_EQUAL(current_user_test, initial_user_count + 1);

	// Validate the DB was updated correctly with the user data
	CU_ASSERT_EQUAL(DB_info[Test1_User.PersonalInfo_Form.id].Age, Test1_User.PersonalInfo_Form.Age);

	//return DB to its initial state
	universal_cleanup();
}

/************************************************************************************
* Test ID                : Create_Account_With_Min_Name_Length
* Description            : Testing Add account Functionality
* Pre-requisites         : Database is already initialized
* Test inputs            : Test user 1 with Name = "Aya" (min. name length)
* Test Expected output   : The DB should be updated with the previous inputs correctly
* Reason                 : All inputs are valid, and the account should be successfully added to the database
*************************************************************************************/
static void Add_Account_With_Min_Name_Length(void)
{
	// Capture the initial user count
	unsigned int initial_user_count = current_user_test;

	// Valid Name (Min length)
	struct User_Input_Type user_min_name_length = Test1_User;
	strcpy(user_min_name_length.PersonalInfo_Form.name, "Aya");

	// Add test user account
	unsigned char RET = Add_Account(&Test1_User);
	// Assign ID to test user
	Test1_User.PersonalInfo_Form.id = current_user_test - 1;

	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1);

	// User count should be incremented
	CU_ASSERT_EQUAL(current_user_test, initial_user_count + 1);

	// Validate the DB was updated correctly with the user data
	CU_ASSERT_STRING_EQUAL(DB_info[Test1_User.PersonalInfo_Form.id].name, Test1_User.PersonalInfo_Form.name);

	//return DB to its initial state
	universal_cleanup();
}

/************************************************************************************
* Test ID                : Create_Account_With_Min_Password_Length
* Description            : Testing Add account Functionality
* Pre-requisites         : Database is already initialized
* Test inputs            : Test user 1 with Pasword = "Hello123" (min. password length)
* Test Expected output   : The DB should be updated with the previous inputs correctly
* Reason                 : All inputs are valid, and the account should be successfully added to the database
*************************************************************************************/
static void Add_Account_With_Min_Password_Length(void)
{
	// Capture the initial user count
	unsigned int initial_user_count = current_user_test;

	// Valid Name (Min length)
	struct User_Input_Type user_min_password_length = Test1_User;
	strcpy(user_min_password_length.LoginCredentials_Form.LoginCredentials.Password, "Hello123");

	// Add test user account
	unsigned char RET = Add_Account(&Test1_User);
	// Assign ID to test user
	Test1_User.PersonalInfo_Form.id = current_user_test - 1;

	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1);

	// User count should be incremented
	CU_ASSERT_EQUAL(current_user_test, initial_user_count + 1);

	// Validate the DB was updated correctly with the user data
	CU_ASSERT_STRING_EQUAL(DB_LoginCredentials[Test1_User.PersonalInfo_Form.id].Password, Test1_User.LoginCredentials_Form.LoginCredentials.Password);

	//return DB to its initial state
	universal_cleanup();
}

/************************************************************************************
* Test ID                : Create_Account_PasswordRecheck_Mismatch
* Description            : Testing Add account Functionality
* Pre-requisites         : Database is already initialized
* Test inputs            : Test user 1 with Pasword = "PASS1234", Password Recheck = "pass4321"
* Test Expected output   : The DB should reject the user and remain unchanged
* Reason                 : Invalid inputs, passwords don't match
*************************************************************************************/
static void Add_Account_PasswordRecheck_Mismatch(void)
{
	// Capture the initial user count
	unsigned int initial_user_count = current_user_test;

	// Valid Name (Min length)
	struct User_Input_Type user_password_mismatch = Test1_User;
	strcpy(user_password_mismatch.LoginCredentials_Form.LoginCredentials.Password, "PASS1234");
	strcpy(user_password_mismatch.LoginCredentials_Form.Password_Recheck, "pass1234");

	// Add test user account
	unsigned char RET = Add_Account(&user_password_mismatch);
	// Assign ID to test user
	Test1_User.PersonalInfo_Form.id = current_user_test - 1;

	// Validate addition rejected
	CU_ASSERT_EQUAL(RET, 0);

	// User count should remain the same
	CU_ASSERT_EQUAL(current_user_test, initial_user_count);

	//return DB to its initial state
	universal_cleanup();
}

/************************************************************************************
* Test ID                : Create_Account_Age_Matches_DOB
* Description            : Testing if the age matches the provided DOB year
* Pre-requisites         : Database is already initialized
* Test inputs            : Test user 1 with valid age and DOB year
* Test Expected output   : The DB should be updated correctly if age matches the DOB, rejected if not
* Reason                 : Ensure that the provided age corresponds correctly to the DOB year
*************************************************************************************/
static void Add_Account_Age_Matches_DOB(void)
{
	// Capture the initial user count
	unsigned int initial_user_count = current_user_test;

	// Valid user where age and DOB year match
	struct User_Input_Type user_dob_age_match = Test1_User;

	// Set the age and DOB year
	user_dob_age_match.PersonalInfo_Form.Age = 30;
	user_dob_age_match.PersonalInfo_Form.DOB_year = CURRENT_YEAR - 30; // Age matches DOB

	// Add test user account
	unsigned char RET = Add_Account(&user_dob_age_match);

	// Assign ID to test user
	user_dob_age_match.PersonalInfo_Form.id = current_user_test - 1;

	// Validate if the account is added successfully
	CU_ASSERT_EQUAL(RET, 1);

	// Validate the DB was updated correctly with the user data
	CU_ASSERT_EQUAL(DB_info[user_dob_age_match.PersonalInfo_Form.id].Age, user_dob_age_match.PersonalInfo_Form.Age);

	// User count should be incremented
	CU_ASSERT_EQUAL(current_user_test, initial_user_count + 1);

	// Clean up and return DB to initial state
	universal_cleanup();
}

/************************************************************************************
* Test ID                : Create_Account_Empty_DB
* Description            : Add user to an empty database (initial state)
* Pre-requisites         : Database is empty
* Test inputs            : Test user 1
* Test Expected output   : The DB should be updated correctly
* Reason                 : All inputs are valid, and the account should be successfully added to the database
*************************************************************************************/
static void Add_Account_Empty_DB(void)
{
	// Clear database
	while (current_user_test > 0) {
		int user_id_to_delete = current_user_test - 1;
		CU_ASSERT_EQUAL(DBM_Delete_User(user_id_to_delete), 1);
	}
	// Validate database is empty
	CU_ASSERT_EQUAL(current_user_test, 0);

	// Add test user account
	unsigned char RET = Add_Account(&Test1_User);

	// Assign ID to test user
	Test1_User.PersonalInfo_Form.id = current_user_test - 1;

	// Validate if the account is added successfully
	CU_ASSERT_EQUAL(RET, 1);

	// User count should be incremented
	CU_ASSERT_EQUAL(current_user_test, 1);

	// Validate the DB was updated correctly with the user data
	unsigned int Test_id = Test1_User.PersonalInfo_Form.id;
	CU_ASSERT_EQUAL(DB_info[Test_id].Age, Test1_User.PersonalInfo_Form.Age);
	CU_ASSERT_EQUAL(DB_info[Test_id].DOB_day, Test1_User.PersonalInfo_Form.DOB_day);
	CU_ASSERT_EQUAL(DB_info[Test_id].DOB_month, Test1_User.PersonalInfo_Form.DOB_month);
	CU_ASSERT_EQUAL(DB_info[Test_id].DOB_year, Test1_User.PersonalInfo_Form.DOB_year);
	CU_ASSERT_EQUAL(DB_info[Test_id].educational_status, Test1_User.PersonalInfo_Form.educational_status);
	CU_ASSERT_EQUAL(DB_info[Test_id].gender, Test1_User.PersonalInfo_Form.gender);
	CU_ASSERT_STRING_EQUAL(DB_info[Test_id].name, Test1_User.PersonalInfo_Form.name);
	CU_ASSERT_STRING_EQUAL(DB_LoginCredentials[Test_id].User_Name, Test1_User.LoginCredentials_Form.LoginCredentials.User_Name);
	CU_ASSERT_STRING_EQUAL(DB_LoginCredentials[Test_id].Password, Test1_User.LoginCredentials_Form.LoginCredentials.Password);

	// Clean up and return DB to initial state
	universal_cleanup();

}

/************************************************************************************
* Test ID                : Create_Account_Same_Username
* Description            : Verifying that the system does not allow adding two users with the same username.
* Pre-requisites         : Database is already initialized and contains no user with the same username.
* Test inputs            : Test user 1 with username "user123", Test user 2 with username "user123"
* Test Expected output   : The DB should only contain one user with the username "user123". The second user should be rejected.
* Reason                 : Ensures that usernames are unique in the system.
*************************************************************************************/
static void Add_Account_Same_Username(void)
{
	// Capture the initial user count
	unsigned int initial_user_count = current_user_test;

	// Add first test user with a unique username
	struct User_Input_Type user_1 = Test1_User;
	strcpy(user_1.LoginCredentials_Form.LoginCredentials.User_Name, "user1234");

	// Add the first user account
	unsigned char RET_1 = Add_Account(&user_1);

	// Assign ID to test user 1
	user_1.PersonalInfo_Form.id = current_user_test - 1;

	// Validate the first user is added successfully
	CU_ASSERT_EQUAL(RET_1, 1);
	CU_ASSERT_EQUAL(current_user_test, initial_user_count + 1);

	// Add second test user with the same username
	struct User_Input_Type user_2 = Test2_User;
	strcpy(user_2.LoginCredentials_Form.LoginCredentials.User_Name, "user1234");

	// Try to add the second user account
	unsigned char RET_2 = Add_Account(&user_2);

	// Validate the second user addition is rejected
	CU_ASSERT_EQUAL(RET_2, 0);
	CU_ASSERT_EQUAL(current_user_test, initial_user_count + 1);  // User count should remain the same

	// Validate that only one user with the username "user1234" exists
	unsigned int id_1 = user_1.PersonalInfo_Form.id;
	CU_ASSERT_STRING_EQUAL(DB_LoginCredentials[id_1].User_Name, "user1234");
}

/************************************************************************************
* Test ID                : Delete_Account_WithValidData
* Description            : Testing delete Account with Valid Data Functionality
* Pre-requisites         : A valid test user (e.g., "Salma") is already present in the database
* Test inputs            : Test 1 User valid ID = current_user_test - 1
* Test Expected output   : The user should be successfully deleted, and the database count should decrease
* Reason                 : Deleting a valid user should update the database correctly
*************************************************************************************/
static void Delete_Account_WithValidData(void)
{
	// Record the initial user count
	unsigned int initial_user_count = current_user_test;

	// Add test user to DB
	unsigned char RET = Add_Account(&Test1_User);
	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1);

	// Validate user count is incremented after addition
	CU_ASSERT_EQUAL(current_user_test, initial_user_count + 1);

	// Assign ID to test user
	Test1_User.PersonalInfo_Form.id = current_user_test - 1;
	// Capture the test user's ID
	unsigned int test_user_id = Test1_User.PersonalInfo_Form.id;

	// Delete the test user
	unsigned char result = Delete_Account(test_user_id);

	// Validate deletion success
	CU_ASSERT_EQUAL(result, 1);

	// Validate user count is decremented after deletion
	CU_ASSERT_EQUAL(current_user_test, initial_user_count);

	// Validate that the test user is no longer in the database
	CU_ASSERT_STRING_NOT_EQUAL(DB_info[test_user_id].name, Test1_User.PersonalInfo_Form.name);

	//return DB to its initial state
	universal_cleanup();
}

/************************************************************************************
* Test ID                : Delete_Account_ID_Boundaries
* Description            : Testing delete Account Functionality
* Pre-requisites         : Database is initialized
* Test inputs            : Test 1 User valid ID = MAX_USERS
*                          User with ID = 0
* Test Expected output   : Both users should be successfully deleted, and the database count should decrease
* Reason                 : Deleting a valid user should update the database correctly
*************************************************************************************/
static void Delete_Account_ID_Boundaries(void)
{
	// Record the initial user count
	unsigned int initial_user_count = current_user_test;

	// Add test user to DB
	unsigned char RET = Add_Account(&Test1_User);
	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1);

	// Validate user count is incremented after addition
	CU_ASSERT_EQUAL(current_user_test, initial_user_count + 1);

	// Assign Max ID to test user
	Test1_User.PersonalInfo_Form.id = MAX_USERS;
	// Capture the test user's ID
	unsigned int test_user_id = Test1_User.PersonalInfo_Form.id;

	// Delete the test user
	unsigned char result = Delete_Account(test_user_id);

	// Validate deletion success
	CU_ASSERT_EQUAL(result, 1);

	// Validate user count is decremented after deletion
	CU_ASSERT_EQUAL(current_user_test, initial_user_count);

	// Validate that the test user is no longer in the database
	CU_ASSERT_STRING_NOT_EQUAL(DB_info[test_user_id].name, Test1_User.PersonalInfo_Form.name);

	// Attempt to Delete the first user 
	unsigned char result_first = Delete_Account(0);

	// Validate deletion success
	CU_ASSERT_EQUAL(result_first, 1);

	// Validate user count is decremented after deletion
	CU_ASSERT_EQUAL(current_user_test, initial_user_count - 1);

	//return DB to its initial state
	universal_cleanup();
}

/************************************************************************************
* Test ID                : Delete_Account_ID_OutOfBound
* Description            : Testing delete Account Functionality
* Pre-requisites         : Database is initialized
* Test inputs            : Test 1 User invalid ID = -1
*                          Test 1 User invalid ID = MAX_USERS + 1
* Test Expected output   : The function should reject the deletion attempt
* Reason                 : The ID input is invalid (out of bounds)
*************************************************************************************/
static void Delete_Account_ID_OutOfBound(void)
{
	// Add test user to DB
	unsigned char RET = Add_Account(&Test1_User);
	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1);

	// Record the user count after addition
	unsigned int user_count_after_addition = current_user_test;

	// Validate user count is incremented after addition
	//CU_ASSERT_EQUAL(current_user_test, user_count_after_addition);

	// Scenario 1: Assign invalid ID test user: MAX_USERS + 1
	Test1_User.PersonalInfo_Form.id = MAX_USERS + 1;
	// Capture the test user's ID
	unsigned int test_user_id_1 = Test1_User.PersonalInfo_Form.id;

	// Attempt to Delete the test user
	unsigned char result_1 = Delete_Account(test_user_id_1);

	// Validate deletion failure
	CU_ASSERT_EQUAL(result_1, 0);

	// Validate user count is the same after addition
	CU_ASSERT_EQUAL(current_user_test, user_count_after_addition);

	// Scenario 2: Assign invalid ID test user: -1
	Test1_User.PersonalInfo_Form.id = -1;
	// Capture the test user's ID
	unsigned int test_user_id_2 = Test1_User.PersonalInfo_Form.id;

	// Attempt to Delete the test user
	unsigned char result_2 = Delete_Account(test_user_id_2);

	// Validate deletion failure
	CU_ASSERT_EQUAL(result_2, 0);

	// Validate user count is the same after addition
	CU_ASSERT_EQUAL(current_user_test, user_count_after_addition);

	//return DB to its initial state
	universal_cleanup();
}

/************************************************************************************
* Test ID                : Delete_Account_SameUser
* Description            : Testing delete Account Functionality
* Pre-requisites         : Database is initialized
* Test inputs            : Test 1 User ID 
* Test Expected output   : The function should accept deletion of user the first time and reject the second time 
* Reason                 : The first time the user has a valid id, the second time user doesn't exist 
*************************************************************************************/
static void Delete_Account_SameUser(void)
{
	// Record the initial user count
	unsigned int initial_user_count = current_user_test;

	// Add test user to DB
	unsigned char RET = Add_Account(&Test1_User);
	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1);

	// Validate user count is incremented after addition
	CU_ASSERT_EQUAL(current_user_test, initial_user_count + 1);

	// Assign ID to test user
	Test1_User.PersonalInfo_Form.id = current_user_test - 1;
	// Capture the test user's ID
	unsigned int test_user_id = Test1_User.PersonalInfo_Form.id;

	// Delete the test user
	unsigned char result_1 = Delete_Account(test_user_id);

	// Validate deletion success
	CU_ASSERT_EQUAL(result_1, 1);

	// Validate user count decremented after deletion
	CU_ASSERT_EQUAL(current_user_test, initial_user_count);

	// Attempt to Delete the test user again
	unsigned char result_2 = Delete_Account(test_user_id);

	// Validate deletion failure
	CU_ASSERT_EQUAL(result_2, 0);

	// Validate user count didn't change
	CU_ASSERT_EQUAL(current_user_test, initial_user_count);

}

/****************************************************************** Course Registration Test Cases ***************************************************************/

/************************************************************************************
* Test ID                : AddStudentToCourse_WithValidData_And_ReEnrollmentCheck
* Description            : Tests registering a valid user to a valid course.
* Pre-requisites         : The user exists in the database, and the course exists.
* Test inputs            : CourseID            -> 1
*                          Test UserID         -> current_user_test - 1
* Test Expected output   : The user should be successfully added to the course, and the enrollment should be reflected in the Enrollments array.
* Reason                 : All inputs are valid, the user should be successfully registered to the course
*************************************************************************************/
static void AddStudentToCourse_WithValidData_And_ReEnrollmentCheck(void)
{
	// Add a test user to the database
	unsigned char RET = DBM_Add_User(&Test1_User);
	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1);

	// Assign ID to the test user
	Test1_User.PersonalInfo_Form.id = current_user_test - 1;
	unsigned int test_user_id = Test1_User.PersonalInfo_Form.id;

	// Example course IDs
	int course_1 = 1;

	// Before enrolling, assert the user is NOT already enrolled
	CU_ASSERT_EQUAL(Enrollments[test_user_id][course_1 - 1], 0);

	// Scenario 1: Register test user to course 1
	unsigned char result_1 = AddStudentToCourse(course_1, test_user_id);

	// Registeration should be successful
	CU_ASSERT_EQUAL(result_1, Enrolled); 
	// Enrollment should be recorded
	CU_ASSERT_EQUAL(Enrollments[test_user_id][course_1 - 1], 1);

	// Scenario 2: Attempt to register test user to course 1 again
	unsigned char result_2 = AddStudentToCourse(course_1, test_user_id);

	// Student shouldn't be registerd again
	CU_ASSERT_EQUAL(result_2, AlreadyEnrolled);
	// Student should remain enrolled
	CU_ASSERT_EQUAL(Enrollments[test_user_id][course_1 - 1], 1);

	//return DB to its initial state
	universal_cleanup();
}

/************************************************************************************
* Test ID                : AddStudentToCourse_FullCourseCapacity
* Description            : Tests registering a user to a course that has reached its maximum capacity.
* Pre-requisites         : The user exists in the database, and the course exists.
* Test inputs            : CourseID            -> 1
*                          Test UserID         -> current_user_test - 1
* Test Expected output   : The user should not be enrolled in the course, and the course capacity should remain unchanged.
* Reason                 : The course is at full capacity
*************************************************************************************/
static void AddStudentToCourse_FullCourseCapacity(void)
{
	// Add user to the database
	unsigned char RET = DBM_Add_User(&Test1_User);
	CU_ASSERT_EQUAL(RET, 1);

	// Assign ID to the test user
	Test1_User.PersonalInfo_Form.id = 6;
	unsigned int test_user_id = Test1_User.PersonalInfo_Form.id;

	// Enroll users in the course until it is full (course initially empty)
	int course_id = 2;
	unsigned char result;

	// First, enroll users to fill the course
	for (int i = 0; i < MAX_STUDENTS_PER_COURSE; i++) {
		result = AddStudentToCourse(course_id, i);        // Enroll a user with index i
		CU_ASSERT_EQUAL(result, Enrolled);               // Ensure that the enrollment was successful
	}

	// Now, try to enroll the next user (which should fail as the course is full)
	result = AddStudentToCourse(course_id, test_user_id);

	// The enrollment should fail
	CU_ASSERT_EQUAL(result, CapacityCompleted);

	// Verify the course is still at its maximum capacity
	int current_enrollment_count = 0;
	for (int i = 0; i < MAX_USERS; i++) {
		if (Enrollments[i][course_id - 1] == TRUE) {
			current_enrollment_count++;
		}
	}
	// Ensure no overflow
	CU_ASSERT_EQUAL(current_enrollment_count, MAX_STUDENTS_PER_COURSE);

	//return DB to its initial state
	universal_cleanup();
}

/************************************************************************************
* Test ID                : AddStudentToCourse_InvalidCourseID
* Description            : Tests registering a valid user to an invalid course.
* Pre-requisites         : The user exists in the database, and the course doesn't exist.
* Test inputs            : CourseID            -> MAX_COURSES + 1 
*                          Test UserID         -> current_user_test - 1
* Test Expected output   : The user should not be registered to the course.
* Reason                 : The course ID doesn't exist
*************************************************************************************/
static void AddStudentToCourse_InvalidCourseID(void)
{
	// Add a test user to the database
	unsigned char RET = DBM_Add_User(&Test1_User);
	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1);

	// Assign Valid ID to the test user
	Test1_User.PersonalInfo_Form.id = current_user_test - 1;
	unsigned int test_user_id = Test1_User.PersonalInfo_Form.id;

	// Invalid Course ID
	int course_1 = MAX_COURSES + 1;

	// Register test user to course 1
	unsigned char result = AddStudentToCourse(course_1, test_user_id);

	// Registeration should fail
	CU_ASSERT_EQUAL(result, 0);
	// Enrollment should not be recorded
	CU_ASSERT_EQUAL(Enrollments[test_user_id][course_1 - 1], 0);

	//return DB to its initial state
	universal_cleanup();
}

/************************************************************************************
* Test ID                : AddStudentToCourse_InvalidStudentID
* Description            : Tests registering an invalid user to a valid course.
* Pre-requisites         : The user exists in the database, and the course exists.
* Test inputs            : CourseID            -> 1
*                          Test UserID         -> MAX_USERS + 1
* Test Expected output   : The user should not be registered to the course.
* Reason                 : The course ID doesn't exist
*************************************************************************************/
static void AddStudentToCourse_InvalidStudentID(void)
{
	// Add a test user to the database
	unsigned char RET = DBM_Add_User(&Test1_User);
	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1);

	// Assign Invalid ID to the test user
	Test1_User.PersonalInfo_Form.id = MAX_USERS + 1;
	unsigned int test_user_id = Test1_User.PersonalInfo_Form.id;

	// Valid Course ID
	int course_1 = 1;

	// Register test user to course 1
	unsigned char result = AddStudentToCourse(course_1, test_user_id);

	// Registeration should fail
	CU_ASSERT_EQUAL(result, 0);
	// Enrollment should not be recorded
	CU_ASSERT_EQUAL(Enrollments[test_user_id][course_1 - 1], 0);

	//return DB to its initial state
	universal_cleanup();
}

/************************************************************************ Login Test Cases ***********************************************************************/

/************************************************************************************
* Test ID                : Login_Detect_User_Type_Valid_Admin
* Description            : Tests the detection of user type when the input is for AdminMohamedTarek (0)
* Pre-requisites         : Simulated input functions are correctly implemented.
* Test inputs            : 0 (AdminMohamedTarek)
* Test Expected output   : The function returns AdminMohamedTarek.
* Reason                 : Input is valid
*************************************************************************************/
static void Login_Detect_User_Type_Admin(void)
{
	// Simulate input for 0: AdminMohamedTarek
	simulate_input("0\n");

	// Call the function to test
	unsigned char result = Detect_User_Type();

	// Assert the expected result
	CU_ASSERT_EQUAL(result, AdminMohamedTarek);

	// cleanup simulated input
	cleanup_input();
}

/************************************************************************************
* Test ID                : Login_Detect_User_Type_Valid_Normal_User
* Description            : Tests the detection of user type when the input is for NormalUser (1) 
* Pre-requisites         : Simulated input functions are correctly implemented.
* Test inputs            : 1 (NormalUser)
* Test Expected output   : The function returns NormalUser.
* Reason                 : Input is valid
*************************************************************************************/
static void Login_Detect_User_Type_Normal_User(void)
{
	// Simulate input for 1: NormalUser
	simulate_input("1\n");

	// Call the function to test
	unsigned char result = Detect_User_Type();

	// Assert the expected result
	CU_ASSERT_EQUAL(result, NormalUser);

	// cleanup simulated input
	cleanup_input();
}

/************************************************************************************
* Test ID                : Login_Detect_User_Type_IncorrectLogin
* Description            : Tests Login user type Incorrect
* Pre-requisites         : Simulated input functions are correctly implemented.
* Test inputs            : 2 (IncorrectLogin)
* Test Expected output   : The function returns IncorrectLogin.
* Reason                 : Input is invalid
*************************************************************************************/
static void Login_Detect_User_Type_IncorrectLogin(void)
{
	// Simulate input for any other value eg.2: IncorrectLogin
	simulate_input("2\n");

	// Call the function to test
	unsigned char result = Detect_User_Type();

	// Assert the expected result
	CU_ASSERT_EQUAL(result, IncorrectLogin);

	// cleanup simulated input
	cleanup_input();
}

/************************************************************************************
* Test ID                : Login_Verify_Admin_Correct_Token
* Description            : Tests verifying the admin login with the correct token.
* Pre-requisites         : The admin token is predefined as 10203040 in the code.
* Test inputs            : Token: 10203040
* Test Expected output   : The function should return TRUE indicating that the admin login is successful.
* Reason                 : The admin token should match the predefined value in the source code.
*************************************************************************************/
static void Login_Verify_Admin_Correct_Token(void)
{
	// Simulate input for Correct Token: 10203040
	simulate_input("10203040\n");

	// Call the function to test
	unsigned char result = Verify_Admin();

	// Assert the expected result
	CU_ASSERT_EQUAL(result, TRUE);

	// cleanup simulated input
	cleanup_input();
}

/************************************************************************************
* Test ID                : Login_Verify_Admin_Incorrect_Token_Two_Attempts
* Description            : Tests verifying the admin login with two incorrect token attempts followed by the correct token.
* Pre-requisites         : The admin token is predefined as 10203040 in the code.
* Test inputs            : Incorrect Token: 12345 
*                          Incorrect Token: 37829
*                          Correct Token  : 10203040
* Test Expected output   : The function should return TRUE indicating that the admin login is successful after the third attempt.
* Reason                 : The admin token should match the predefined value in the source code. The function should allow up to 3 attempts.
*************************************************************************************/
static void Login_Verify_Admin_Incorrect_Token_Two_Attempts(void)
{
	// Simulate the sequence of inputs for the token attempts
	// First Try: Simulate input for Incorrect Token: 12345
	// Second Try: Simulate input for Incorrect Token: 37829
	// Third Try: Simulate input for Correct Token: 10203040
	simulate_input("12345\n37829\n10203040\n");

	// Call the function to test
	unsigned char result = Verify_Admin();

	// Assert the expected result
	CU_ASSERT_EQUAL(result, TRUE);

	// cleanup simulated input
	cleanup_input();
}

/************************************************************************************
* Test ID                : Login_Verify_Admin_Incorrect_Token_Three_Attempts
* Description            : Tests verifying the admin login when three incorrect token attempts are made.
* Pre-requisites         : The admin token is predefined as 10203040 in the code.
* Test inputs            : Incorrect Token: 12345
*                          Incorrect Token: 37829
*                          Incorrect Token: 0403021
* Test Expected output   : The function should return FALSE indicating that the admin login failed after three incorrect attempts.
* Reason                 : The admin token should match the predefined value in the source code. The function should reject login after three failed attempts.
*************************************************************************************/
static void Login_Verify_Admin_Incorrect_Token_Three_Attempts(void)
{
	// Simulate the sequence of inputs for the token attempts
	// First Try: Simulate input for Incorrect Token: 12345
	// Second Try: Simulate input for Incorrect Token: 37829
	// Third Try: Simulate input for Correct Token: 0403021
	
	simulate_input("12345\n37829\n0403021\n");

	// Call the function to test
	unsigned char result = Verify_Admin();

	// Assert the expected result
	CU_ASSERT_EQUAL(result, FALSE);

	// cleanup simulated input
	cleanup_input();
}

/************************************************************************************
* Test ID                : Login_Verify_User_Valid_Credentials
* Description            : Tests verifying the login of a user with valid credentials.
* Pre-requisites         : The DBM_Add_User function should successfully add a test user.
* Test inputs            : Username: "EdgesAcademy"
*                          Password: "Edges123"
* Test Expected output   : The function should return Login_Successful, and the id_ptr should match the added user's ID.
* Reason                 : The function should validate the credentials correctly and return a successful login.
*************************************************************************************/
static void Login_Verify_User_Valid_Credentials(void)
{
	// Add a test user to the database
	unsigned char RET = DBM_Add_User(&Test1_User);
	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1);

	// Assign ID to the test user
	Test1_User.PersonalInfo_Form.id = current_user_test - 1;
	unsigned int user_id = Test1_User.PersonalInfo_Form.id;

	char* username = "EdgesAcademy";
	char* password = "Edges123";
	int id_ptr = -1;

	// Run the Verify_User function
	unsigned char result = Verify_User(username, password, &id_ptr);

	// Should return successful login
	CU_ASSERT_EQUAL(result, Login_Successful);
	// ID should match the added user
	CU_ASSERT_EQUAL(id_ptr, user_id);          

	//return DB to its initial state
	universal_cleanup();
}

/************************************************************************************
* Test ID                : Login_Verify_User_Invalid_Password
* Description            : Tests verifying the login of a user with an incorrect password.
* Pre-requisites         : The DBM_Add_User function should successfully add a test user.
* Test inputs            : Username: "EdgesAcademy" 
*                          Password: "IncorrectPassword"
* Test Expected output   : The function should return Password_incorrect, and the id_ptr should be -1.
* Reason                 : The system should return a password incorrect message and set the user ID to -1.
*************************************************************************************/
static void Login_Verify_User_Invalid_Password(void)
{
	// Add a test user to the database
	unsigned char RET = DBM_Add_User(&Test1_User);
	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1);

	// Assign ID to the test user
	Test1_User.PersonalInfo_Form.id = current_user_test - 1;
	unsigned int user_id = Test1_User.PersonalInfo_Form.id;

	char* username = "EdgesAcademy";
	char* password = "IncorrectPassword";
	int id_ptr = -1;

	// Run the Verify_User function
	unsigned char result = Verify_User(username, password, &id_ptr);

	// Should return Password incorrect
	CU_ASSERT_EQUAL(result, Password_incorrect);
	// ID should remain -1
	CU_ASSERT_EQUAL(id_ptr, -1);

	//return DB to its initial state
	universal_cleanup();
}

/************************************************************************************
* Test ID                : Login_Verify_User_Nonexistent_Username
* Description            : Tests verifying the login of a user with a nonexistent username.
* Pre-requisites         : The DBM_Add_User function should successfully add a test user.
* Test inputs            : Username: "NonexistentUsername"
*                          Password: "Edges123"
* Test Expected output   : The function should return UserName_NotFound, and the id_ptr should remain -1.
* Reason                 : The system should return a UserName_NotFound error when attempting to log in with a username that does not exist.
*************************************************************************************/
static void Login_Verify_User_Nonexistent_Username(void)
{
	// Add a test user to the database
	unsigned char RET = DBM_Add_User(&Test1_User);
	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1);

	// Assign ID to the test user
	Test1_User.PersonalInfo_Form.id = current_user_test - 1;
	unsigned int user_id = Test1_User.PersonalInfo_Form.id;

	char* username = "NonexistentUsername";
	char* password = "Edges123";
	int id_ptr = -1;

	// Run the Verify_User function
	unsigned char result = Verify_User(username, password, &id_ptr);

	// Should return Password incorrect
	CU_ASSERT_EQUAL(result, UserName_NotFound);
	// ID should remain -1
	CU_ASSERT_EQUAL(id_ptr, -1);

	//return DB to its initial state
	universal_cleanup();
}


/*********************************************************************** Backend Test Cases **********************************************************************/

/************************************************************************************
* Test ID                : BackEnd_AdminRunner_AddNewUser_ValidData
* Description            : Test the functionality of adding a new user using valid data through the Admin Runner function.
* Pre-requisites         : Database initialized with DBM_initDB() and contains at least one user.
* Test inputs            : User Choice: A
*                          Valid User Data
*                          User Choice: Q
* Test Expected output   : User count increases by 1.
						   Database contains new user with the provided details.
* Reason                 : Admin Runner is accessible and valid data is entered.
*************************************************************************************/
static void BackEnd_AdminRunner_AddNewUser_ValidData(void)
{
	// Capture the initial user count
	unsigned int initial_user_count = current_user_test;
	printf("\nInitial user count: %d\n", initial_user_count);
	//DBM_PrintUsers();

	// Simulate the sequence of inputs for choice A: Add New Account (Valid Inputs)
	/*Simulated Inputs:
	 **Admin Runner Page:
	 * User Choice: A (Add New Account)
	 * Name: Salma
	 * Age: 25
	 * DOB_Day: 5
	 * DOB_Mont: 7
	 * DOB_Year: 1999
	 * Education: Graduate (3)
	 * Gender: Female (2)
	 * Username: EdgesAcademy
	 * Password: Edges123
	 * Password Recheck: Edges123
	 * User Choice: Q (Quit)
	 */
	simulate_input("A\
                  \nSalma\
                  \n25\
                  \n5\
                  \n7\
                  \n1999\
                  \n3\
                  \n2\
                  \nEdgesAcademy\
                  \nEdges123\
                  \nEdges123\
                  \nQ");

	// Redirect printf output to capture printed messages
	redirect_printf_output();

	// Call the function to test
	Admin_Runner();

	printf("\nUser count after new user was added: %d\n", current_user_test);
	//DBM_PrintUsers();

	// Assert that the output contains the expected message from the else block
	assert_printf_output_contains("**************** User Added Successfully ****************");

	// User count should increase by 1
	CU_ASSERT_EQUAL(current_user_test, initial_user_count + 1);

	// Validate the DB was updated correctly with the user data
	CU_ASSERT_EQUAL(DB_info[current_user_test - 1].Age, 25);
	CU_ASSERT_EQUAL(DB_info[current_user_test - 1].DOB_day, 5);
	CU_ASSERT_EQUAL(DB_info[current_user_test - 1].DOB_month, 7);
	CU_ASSERT_EQUAL(DB_info[current_user_test - 1].DOB_year, 1999);
	CU_ASSERT_EQUAL(DB_info[current_user_test - 1].educational_status, 3);
	CU_ASSERT_EQUAL(DB_info[current_user_test - 1].gender, 2);
	CU_ASSERT_STRING_EQUAL(DB_info[current_user_test - 1].name, "Salma");
	CU_ASSERT_STRING_EQUAL(DB_LoginCredentials[current_user_test - 1].User_Name, "EdgesAcademy");
	CU_ASSERT_STRING_EQUAL(DB_LoginCredentials[current_user_test - 1].Password, "Edges123");

	// cleanup simulated input
	cleanup_input();

	// Cleanup the captured printf output after the test
	cleanup_printf_output();

	//return DB to its initial state
	universal_cleanup();

}

/************************************************************************************
* Test ID                : BackEnd_AdminRunner_AddNewUser_InvalidData
* Description            : Test the functionality of the Admin Runner when attempting to add a new user with invalid data.
* Pre-requisites         : Database initialized with DBM_initDB() and contains at least one user.
* Test inputs            : User Choice: D
*                          InValid User Data
*                          User Choice: Q
* Test Expected output   : Print the message: "Wrong inputs".
*                          User count remains the same.
*                          Database is not updated with new user details.
* Reason                 : Admin Runner is accessible, and invalid input (mismatched passwords) is entered.
*************************************************************************************/
static void BackEnd_AdminRunner_AddNewUser_InvalidData(void)
{
	// Capture the initial user count
	unsigned int initial_user_count = current_user_test;
	printf("\nInitial user count: %d\n", initial_user_count);
	//DBM_PrintUsers();

	// Simulate the sequence of inputs for choice A: Add New Account (Invalid Inputs: Passwords don't match)
	// Simulate the sequence of inputs for choice A: Add New Account (Invalid Inputs)
	/*Simulated Inputs:
	 **Admin Runner Page:
	 * User Choice: A (Add New Account)
	 * Name: Salma
	 * Age: 25
	 * DOB_Day: 5
	 * DOB_Mont: 7
	 * DOB_Year: 1999
	 * Education: Graduate (3)
	 * Gender: Female (2)
	 * Username: EdgesAcademy
	 * Password: Edges123
	 * Password Recheck: Edges123456
	 * User Choice: Q (Quit)
	 */

	simulate_input("A\
                  \nSalma\
                  \n25\
                  \n5\
                  \n7\
                  \n1999\
                  \n3\
                  \n2\
                  \nEdgesAcademy\
                  \nEdges123\
                  \nEdges123456\
                  \nQ");

	// Redirect printf output to capture printed messages
	redirect_printf_output();

	// Call the function to test
	Admin_Runner();

	printf("\nUser count after attempt to add an invalid user: %d\n", current_user_test);
	//DBM_PrintUsers();

	// Assert that the output contains the expected message from the else block
	assert_printf_output_contains("**************** Wrong inputs ****************");

	// User count should be the same
	CU_ASSERT_EQUAL(current_user_test, initial_user_count);

	// cleanup simulated input
	cleanup_input();

	// Cleanup the captured printf output after the test
	cleanup_printf_output();

	//return DB to its initial state
	universal_cleanup();

}

/************************************************************************************
* Test ID                : BackEnd_AdminRunner_DeleteUser_ValidID
* Description            : Validate the "Delete User" choice (option D) in the Admin Runner function when provided with a valid user ID.
* Pre-requisites         : The test database must be initialized and available and Test User 1 is added.
* Test inputs            : User Choice: D
*                          Valid ID: 3
*                          User Choice: Q
* Test Expected output   : The user with ID 3 is deleted from the database and the user count decreases by 1 after deletion
* Reason                 : Admin Runner is accessible and valid data is entered.
*************************************************************************************/
static void BackEnd_AdminRunner_DeleteUser_ValidID(void)
{	
	// Capture the initial user count
	unsigned int initial_user_count = current_user_test;

	printf("\nInitial user count: %d\n", initial_user_count);
	//DBM_PrintUsers();

	// Add a test user to the database
	unsigned char RET = DBM_Add_User(&Test1_User);
	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1);

	// Validate user count is incremented after addition
	CU_ASSERT_EQUAL(current_user_test, initial_user_count + 1);

	// Assign ID to the test user
	Test1_User.PersonalInfo_Form.id = current_user_test - 1;

	printf("\nUser count after test user was added: %d\n", initial_user_count + 1);
	//DBM_PrintUsers();

	// Simulate the sequence of inputs for choice D: Delete User (Valid ID = 3)
	/*Simulated Inputs:
	 **Admin Runner Page:
	 * User Choice: D (DeleteAccount)
	 * ID: 3
	 * User Choice: Q (Quit)
	 */

	simulate_input("D\
                  \n3\
                  \nQ");

	// Call the function to test
	Admin_Runner();

	printf("\nUser count after deleting the test user: %d\n", current_user_test);
	//DBM_PrintUsers();

	// User count should be the same as the initial count
	CU_ASSERT_EQUAL(current_user_test, initial_user_count);

	// cleanup simulated input
	cleanup_input();

	//return DB to its initial state
	universal_cleanup();

}

/************************************************************************************
* Test ID                : BackEnd_AdminRunner_DeleteUser_InvalidID
* Description            : Validate the "Delete User" choice (option D) in the Admin Runner function when provided with an invalid user ID.
* Pre-requisites         : The test database must be initialized and available and No user should exist with ID -1.
* Test inputs            : User Choice: D
*                          Valid ID: -1
*                          User Choice: Q
* Test Expected output   : No user is deleted.
*                          The user count remains unchanged.
*                          Error message "User ID Doesn't Exist" is printed.
* Reason                 : Invalid ID is provided so no change should occur
*************************************************************************************/
static void BackEnd_AdminRunner_DeleteUser_InvalidID(void)
{
	// Capture the initial user count
	unsigned int initial_user_count = current_user_test;

	printf("\nInitial user count: %d\n", initial_user_count);
	//DBM_PrintUsers();

	// Simulate the sequence of inputs for choice D: Delete User (Invalid ID = -1)
	/*Simulated Inputs:
	 **Admin Runner Page:
	 * User Choice: D (DeleteAccount)
	 * ID: -1
	 * User Choice: Q (Quit)
	 */

	simulate_input("D\
                  \n-1\
                  \nQ");

	// Redirect printf output to capture printed messages
	redirect_printf_output();

	// Call the function to test
	Admin_Runner();

	printf("\nUser count after attempting to delete a user: %d\n", current_user_test);
	//DBM_PrintUsers();

	// Assert that the output contains the expected message from the else block
	assert_printf_output_contains("User ID Doesn't Exist");

	// User count should be the same as the initial count
	CU_ASSERT_EQUAL(current_user_test, initial_user_count);

	// cleanup simulated input
	cleanup_input();

	// Cleanup the captured printf output after the test
	cleanup_printf_output();

	//return DB to its initial state
	universal_cleanup();

}

/************************************************************************************
* Test ID                : BackEnd_AdminRunner_CheckReservation
* Description            : Validate the "Check Course Reservation" choice (option C) in the Admin Runner function.
* Pre-requisites         : Database must contain valid data and DBM_CheckReservations() function should be implemented.
* Test inputs            : User Choice: C (Check Reservation) 
*                          User Choice: Q (Quit)
* Test Expected output   : The system enters the "Check Course Reservation" flow and DBM_CheckReservations() executes successfully.
* Reason                 : Inputs are entered correctly
*************************************************************************************/
static void BackEnd_AdminRunner_CheckReservation(void)
{
	// Simulate the sequence of inputs for choice C: Check Course Reservation
	/*Simulated Inputs:
	 **Admin Runner Page:
	 * User Choice: C (Check Course Reservation)
	 * User Choice: Q (Quit)
	 */
	simulate_input("C\
                  \nQ");

	// Redirect printf output to capture printed messages
	redirect_printf_output();

	// Call the function to test
	Admin_Runner();

	// Assert that the case is entered and the DBM_CheckReservations() function is executed
	assert_printf_output_contains("************* Course ");

	// cleanup simulated input
	cleanup_input();

	// Cleanup the captured printf output after the test
	cleanup_printf_output();

	//return DB to its initial state
	universal_cleanup();

}

/************************************************************************************
* Test ID                : BackEnd_AdminRunner_PrintAllUsers
* Description            : Validate the "Print All Users" choice (option P) in the Admin Runner function.
* Pre-requisites         : Database must contain valid data and DBM_PrintUsers() function should be implemented.
* Test inputs            : User Choice: P (Print All Users) 
*                          User Choice: Q
* Test Expected output   : The system enters the "Print All Users" flow and DBM_PrintUsers function is executed.
* Reason                 : Inputs are entered correctly
*************************************************************************************/
static void BackEnd_AdminRunner_PrintAllUsers(void)
{
	// Simulate the sequence of inputs for choice P: Print All Users
	/*Simulated Inputs:
	 **Admin Runner Page:
	 * User Choice: P (Print All Users)
	 * User Choice: Q (Quit)
	 */
	simulate_input("P\
                  \nQ");

	// Redirect printf output to capture printed messages
	redirect_printf_output();

	// Call the function to test
	Admin_Runner();

	// Assert that the case is entered and the DBM_PrintUsers function is executed
	assert_printf_output_contains("The Data Of User ");
	assert_printf_output_contains("*****************************************************************\n");

	// cleanup simulated input
	cleanup_input();

	// Cleanup the captured printf output after the test
	cleanup_printf_output();

	//return DB to its initial state
	universal_cleanup();

}

/************************************************************************************
* Test ID                : BackEnd_AdminRunner_PrintSpecificUser_Exists
* Description            : Validate the "Print a Specific User" choice (option U) in the Admin Runner function when the user exists in the database.
* Pre-requisites         : The database must be initialized and DBM_Add_User must correctly add the test user, and their ID must be retrievable.
* Test inputs            : User Choice: U (Print a Specific User) 
*                          User ID: 3
*                          User Choice: Q
* Test Expected output   : The system enters the "Print a Specific User" flow amd the DBM_PrintUserDataAdmin function is executed.
* Reason                 : Inputs are entered correctly
*************************************************************************************/
static void BackEnd_AdminRunner_PrintSpecificUser_Exists(void)
{
	// Capture the initial user count
	unsigned int initial_user_count = current_user_test;

	printf("\nInitial user count: %d\n", initial_user_count);
	//DBM_PrintUsers();

	// Add a test user to the database
	unsigned char RET = DBM_Add_User(&Test1_User);
	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1);

	// Validate user count is incremented after addition
	CU_ASSERT_EQUAL(current_user_test, initial_user_count + 1);

	// Assign ID to the test user
	Test1_User.PersonalInfo_Form.id = current_user_test - 1;

	// Simulate the sequence of inputs for choice U: Print a specific User (test user id = 3) 
	/*Simulated Inputs:
	 **Admin Runner Page:
	 * User Choice: U (Print a specific User)
	 * ID: 3
	 * User Choice: Q (Quit)
	 */
	simulate_input("U\
                  \n3\
                  \nQ");

	// Redirect printf output to capture printed messages
	redirect_printf_output();

	// Call the function to test
	Admin_Runner();

	// Assert that the case is entered and the DBM_PrintUserDataAdmen function is executed
	assert_printf_output_contains("Enter the user id you want to print \n");
	assert_printf_output_contains("The Data Of User ");

	// cleanup simulated input
	cleanup_input();

	// Cleanup the captured printf output after the test
	cleanup_printf_output();

	//return DB to its initial state
	universal_cleanup();

}

/************************************************************************************
* Test ID                : BackEnd_AdminRunner_PrintSpecificUser_Nonexistent
* Description            : Validate the "Print a Specific User" choice (option U) in the Admin Runner function when the user does not exist in the database.
* Pre-requisites         : The test database must be initialized and DBM_Add_User must correctly add the test user, and their ID must be retrievable.
* Test inputs            : User Choice: U (Print a Specific User) 
*                          User ID: 100 (Nonexistent ID) 
*                          User Choice: Q
* Test Expected output   : The system enters the "Print a Specific User" flow.
*                          The output contains the message: "User ID Doesn't Exist".
* Reason                 : User ID provided doesn't exist in the DB.
*************************************************************************************/
static void BackEnd_AdminRunner_PrintSpecificUser_Nonexistent(void)
{
	// Capture the initial user count
	unsigned int initial_user_count = current_user_test;

	printf("\nInitial user count: %d\n", initial_user_count);
	//DBM_PrintUsers();

	// Add a test user to the database
	unsigned char RET = DBM_Add_User(&Test1_User);
	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1);

	// Validate user count is incremented after addition
	CU_ASSERT_EQUAL(current_user_test, initial_user_count + 1);

	// Assign ID to the test user
	Test1_User.PersonalInfo_Form.id = current_user_test - 1;

	// Simulate the sequence of inputs for choice U: Print a specific User (ID doesn't exist = 100) 
	/*Simulated Inputs:
	 **Admin Runner Page:
	 * User Choice: U (Print a specific User)
	 * ID: 100
	 * User Choice: Q (Quit)
	 */

	simulate_input("U\
                  \n100\
                  \nQ");

	// Redirect printf output to capture printed messages
	redirect_printf_output();

	// Call the function to test
	Admin_Runner();

	// Assert that the case is entered and the DBM_PrintUserDataAdmen function is executed
	assert_printf_output_contains("Enter the user id you want to print \n");
	assert_printf_output_contains("User ID Doesn't Exist \n");

	// cleanup simulated input
	cleanup_input();

	// Cleanup the captured printf output after the test
	cleanup_printf_output();

	//return DB to its initial state
	universal_cleanup();

}

/************************************************************************************
* Test ID                : BackEnd_AdminRunner_Help
* Description            : Test the "Help" option (choice H) in the Admin Runner function.
* Pre-requisites         : The Admin Runner function must be running and the help option should print the correct message.
* Test inputs            : User Choice: H (Help) 
*                          User Choice: Q (Quit)
* Test Expected output   : The Help menu is displayed.
* Reason                 : Correct choice was selected
*************************************************************************************/
static void BackEnd_AdminRunner_Help(void)
{
	// Simulate the sequence of inputs for choice H: Help
	/*Simulated Inputs:
	 **Admin Runner Page:
	 * User Choice: H (Help)
	 * User Choice: Q (Quit)
	 */
	simulate_input("H\
                  \nQ");

	// Redirect printf output to capture printed messages
	redirect_printf_output();

	// Call the function to test
	Admin_Runner();

	// Assert that the case is executed correctly
	assert_printf_output_contains("Enter your choice and enter Q for Quit\n");
	assert_printf_output_contains("Enter H for Help\n");

	// cleanup simulated input
	cleanup_input();

	// Cleanup the captured printf output after the test
	cleanup_printf_output();

	//return DB to its initial state
	universal_cleanup();

}

/************************************************************************************
* Test ID                : BackEnd_AdminRunner_InvalidChoice
* Description            : Test handling of invalid user choice input (e.g., "Z") in the Admin Runner function.
* Pre-requisites         : The Admin Runner function must be running and the system should handle invalid user inputs correctly.
* Test inputs            : User Choice: Z (Invalid choice) 
*                          User Choice: Q (Quit)
* Test Expected output   : The system displays an error message.
* Reason                 : This test ensures that the program properly rejects invalid inputs and guides the user back to valid choices.
*************************************************************************************/
static void BackEnd_AdminRunner_InvalidChoice(void)
{
	// Simulate the sequence of inputs for an invalid choice eg Z
	/*Simulated Inputs:
	 **Admin Runner Page:
	 * User Choice: Z 
	 * User Choice: Q (Quit)
	 */
	simulate_input("Z\
                  \nQ");

	// Redirect printf output to capture printed messages
	redirect_printf_output();

	// Call the function to test
	Admin_Runner();

	// Assert that the default is executed correctly
	assert_printf_output_contains("Wrong input try again or use H for Help\n");

	// cleanup simulated input
	cleanup_input();

	// Cleanup the captured printf output after the test
	cleanup_printf_output();

	//return DB to its initial state
	universal_cleanup();

}

/************************************************************************************
* Test ID                : BackEnd_CustomerRunner_CreateNewAccount_ValidData
* Description            : Test handling of valid data input for creating a new account in the Customer Runner function.
* Pre-requisites         : The Customer Runner function must be running and the system should accept valid inputs for new account creation.
* Test inputs            : User Choice: C (Create New Account) 
*                          Valid User Data
*                          User Choice: Q (Quit)
* Test Expected output   : The system displays: " User Added Successfully ".
*                          The user count should increase by 1.
*                          The new user should be added to the database with the correct details.
* Reason                 : This test ensures that the new account creation functionality works as expected for valid inputs.
*************************************************************************************/
static void BackEnd_CustomerRunner_CreateNewAccount_ValidData(void)
{
	// Capture the initial user count
	unsigned int initial_user_count = current_user_test;
	printf("\nInitial user count: %d\n", initial_user_count);
	//DBM_PrintUsers();

	// Simulate the sequence of inputs for choice C: Create New Account (Valid Inputs)
	/*Simulated Inputs:
	 **Customer Runner Page:
	 * User Choice: C (Create New Account)
	 * Name: Salma
	 * Age: 25
	 * DOB_Day: 5
	 * DOB_Mont: 7
	 * DOB_Year: 1999
	 * Education: Graduate (3)
	 * Gender: Female (2)
	 * Username: EdgesAcademy
	 * Password: Edges123
	 * Password Recheck: Edges123
	 * User Choice: Q (Quit)
	 */
	simulate_input("C\
                  \nSalma\
                  \n25\
                  \n5\
                  \n7\
                  \n1999\
                  \n3\
                  \n2\
                  \nEdgesAcademy\
                  \nEdges123\
                  \nEdges123\
                  \nQ");

	// Redirect printf output to capture printed messages
	redirect_printf_output();

	// Call the function to test
	Customer_Runner();

	printf("\nUser count after new account was created: %d\n", current_user_test);
	//DBM_PrintUsers();
	
	// Assert that the output contains the expected message from the else block
	assert_printf_output_contains("**************** User Added Successfully ****************");

	// User count should increase by 1
	CU_ASSERT_EQUAL(current_user_test, initial_user_count + 1);

	// Validate the DB was updated correctly with the user data
	CU_ASSERT_EQUAL(DB_info[current_user_test - 1].Age, 25);
	CU_ASSERT_EQUAL(DB_info[current_user_test - 1].DOB_day, 5);
	CU_ASSERT_EQUAL(DB_info[current_user_test - 1].DOB_month, 7);
	CU_ASSERT_EQUAL(DB_info[current_user_test - 1].DOB_year, 1999);
	CU_ASSERT_EQUAL(DB_info[current_user_test - 1].educational_status, 3);
	CU_ASSERT_EQUAL(DB_info[current_user_test - 1].gender, 2);
	CU_ASSERT_STRING_EQUAL(DB_info[current_user_test - 1].name, "Salma");
	CU_ASSERT_STRING_EQUAL(DB_LoginCredentials[current_user_test - 1].User_Name, "EdgesAcademy");
	CU_ASSERT_STRING_EQUAL(DB_LoginCredentials[current_user_test - 1].Password, "Edges123");

	// cleanup simulated input
	cleanup_input();

	// Cleanup the captured printf output after the test
	cleanup_printf_output();

	//return DB to its initial state
	universal_cleanup();

}

/************************************************************************************
* Test ID                : BackEnd_CustomerRunner_CreateNewAccount_InvalidData
* Description            : Test handling of invalid data input (Age: 105) for creating a new account in the Customer Runner function.
* Pre-requisites         : The Customer Runner function must be running and the system should reject invalid data (e.g., age above a valid limit).
* Test inputs            : User Choice: C (Create New Account) 
*                          Invalid User Data (Age 105)
*                          User Choice: Q (Quit)
* Test Expected output   : The system displays: "Wrong inputs".
                           The user count should remain the same.
* Reason                 : The user provided invalid input for the age field.
*************************************************************************************/
static void BackEnd_CustomerRunner_CreateNewAccount_InvalidData(void)
{
	// Capture the initial user count
	unsigned int initial_user_count = current_user_test;
	printf("\nInitial user count: %d\n", initial_user_count);
	//DBM_PrintUsers();

	// Simulate the sequence of inputs for choice C: Create New Account (Invalid Inputs: Age: 105)
	/*Simulated Inputs:
	 **Customer Runner Page:
	 * User Choice: C (Create New Account)
	 * Name: Salma
	 * Age: 105
	 * DOB_Day: 5
	 * DOB_Mont: 7
	 * DOB_Year: 1999
	 * Education: Graduate (3)
	 * Gender: Female (2)
	 * Username: EdgesAcademy
	 * Password: Edges123
	 * Password Recheck: Edges123
	 * User Choice: Q (Quit)
	 */
	simulate_input("C\
                  \nSalma\
                  \n105\
                  \n5\
                  \n7\
                  \n1999\
                  \n3\
                  \n2\
                  \nEdgesAcademy\
                  \nEdges123\
                  \nEdges123\
                  \nQ");

	// Redirect printf output to capture printed messages
	redirect_printf_output();

	// Call the function to test
	Customer_Runner();

	printf("\nUser count after attempt to create an invalid account: %d\n", current_user_test);
	//DBM_PrintUsers();

	// Assert that the output contains the expected message from the else block
	assert_printf_output_contains("**************** Wrong inputs ****************");

	// User count should be the same
	CU_ASSERT_EQUAL(current_user_test, initial_user_count);

	// cleanup simulated input
	cleanup_input();

	// Cleanup the captured printf output after the test
	cleanup_printf_output();

	//return DB to its initial state
	universal_cleanup();
}

/************************************************************************************
* Test ID                : BackEnd_CustomerRunner_Login_Successful
* Description            : Test the successful login flow in the Customer Runner function. 
*                          The test ensures that a user can log in using valid credentials and then log out successfully
* Pre-requisites         : A valid test user (Test1_User) must be added to the database.
*                          The user must provide the correct login credentials.
* Test inputs            : User Choice: L (Login) 
*                          Username: EdgesAcademy 
*                          Password: Edges123 User Choice: 
*                          User Choice: O (Logout) 
*                          User Choice: Q (Quit)
* Test Expected output   : The system should welcome the user to the homepage after a successful login.
*                          The user should be able to log out successfully.
* Reason                 : The user provided correct login credentials
*************************************************************************************/
static void BackEnd_CustomerRunner_Login_Successful(void)
{
	// Capture the initial user count
	unsigned int initial_user_count = current_user_test;
	
	// Add a test user to the database
	unsigned char RET = DBM_Add_User(&Test1_User);
	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1);

	// Validate user count is incremented after addition
	CU_ASSERT_EQUAL(current_user_test, initial_user_count + 1);

	// Simulate the sequence of inputs for choice L: Login (Successful)
	/*Simulated Inputs:
	 **Customer Runner Page: 
	 * User Choice: L (Login)
	 * Username: EdgesAcademy
	 * Password: Edges123
	 **After being redirected to Homepage:
	 * User Choice: O (Logout)
	 * User Choice: Q (Quit)
	 */
	simulate_input("L\
                  \nEdgesAcademy\
                  \nEdges123\
                  \nO\
                  \nQ");

	// Redirect printf output to capture printed messages
	redirect_printf_output();

	// Call the function to test
	Customer_Runner();

	// Assert that user was redirected to the Home Page
	assert_printf_output_contains("Welcome To Your Home Page");

	// cleanup simulated input
	cleanup_input();

	// Cleanup the captured printf output after the test
	cleanup_printf_output();

	//return DB to its initial state
	universal_cleanup();

}

/************************************************************************************
* Test ID                : BackEnd_CustomerRunner_Login_UsernameNotFound
* Description            : Test the scenario where a user attempts to log in with an invalid username.
*                          The test ensures that the system handles non-existent usernames correctly by displaying the appropriate error message.
* Pre-requisites         : A valid test user (Test1_User) must be added to the database.
*                          The user must provide an incorrect username.
* Test inputs            : User Choice: `L` (Login)
*                          Username: `WrongUsername`
*                          Password: `Edges123`
*                          User Choice: `Q` (Quit)
* Test Expected output   : The system should display "User Name Doesn't Exist Try Again".
* Reason                 : This test ensures that the system properly handles a scenario where the username is not found in the database.
*************************************************************************************/

static void BackEnd_CustomerRunner_Login_UsernameNotFound(void)
{
	// Capture the initial user count
	unsigned int initial_user_count = current_user_test;

	// Add a test user to the database
	unsigned char RET = DBM_Add_User(&Test1_User);
	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1);

	// Validate user count is incremented after addition
	CU_ASSERT_EQUAL(current_user_test, initial_user_count + 1);

	// Simulate the sequence of inputs for choice L: Login (Username Not Found)
	/*Simulated Inputs:
	 **Customer Runner Page:
	 * User Choice: L (Login)
	 * Username: WrongUsername
	 * Password: Edges123
	 User Choice: Q (Quit)
	 */
	simulate_input("L\
                  \nWrongUsername\
                  \nEdges123\
                  \nQ");

	// Redirect printf output to capture printed messages
	redirect_printf_output();

	// Call the function to test
	Customer_Runner();

	// Assert that the username doesn't exist
	assert_printf_output_contains("User Name Doesn't Exist Try Again");

	// cleanup simulated input
	cleanup_input();

	// Cleanup the captured printf output after the test
	cleanup_printf_output();

	//return DB to its initial state
	universal_cleanup();

}

/************************************************************************************
* Test ID                : BackEnd_CustomerRunner_Login_PasswordIncorrect_FirstTrial
* Description            : Verifies login functionality when the password is incorrect on the first attempt and then correct on the second.
* Pre-requisites         : Test1_User added to the database
* Test inputs            : User Choice: L (Login)
*                          Username: EdgesAcademy
*                          Password (Trial 1): WrongPassFirstTrial
*                          Password (Trial 2): Edges123
*                          User Choice: O (Logout)
*                          User Choice: Q (Quit)
*                          After returning to Customer Runner: User Choice: Q (Quit)
* Test Expected output   : "Password incorrect, Enter Your Password again. 2 Trials Left"
*                          "Welcome To Your Home Page"
* Reason                 : To verify login functionality when the password is incorrect on the first attempt and correct on the second.
*************************************************************************************/
static void BackEnd_CustomerRunner_Login_PasswordIncorrect_FirstTrial(void)
{
	// Capture the initial user count
	unsigned int initial_user_count = current_user_test;

	// Add a test user to the database
	unsigned char RET = DBM_Add_User(&Test1_User);
	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1);

	// Validate user count is incremented after addition
	CU_ASSERT_EQUAL(current_user_test, initial_user_count + 1);

	// Simulate the sequence of inputs for choice L: Login (Incorrect 1st trial)
	/*Simulated Inputs:
	 **Customer Runner Page:
	 * User Choice: L (Login)
	 * Username: EdgesAcademy
	 * Password (Trial 1): WrongPassFirstTrial
	 * Password (Trial 2): Edges123
	 **After being redirected to Homepage:
	 * User Choice: O (Logout)
	 * User Choice: Q (Quit)
	 **After returning to Customer Runner Again:
	 User Choice: Q (Quit)
	 */
	simulate_input("L\
                  \nEdgesAcademy\
                  \nWrongPassFirstTrial\
                  \nEdges123\
                  \nO\
                  \nQ\
                  \nQ");

	// Redirect printf output to capture printed messages
	redirect_printf_output();

	// Call the function to test
	Customer_Runner();

	// Assert that the password was wrong upon first trial
	assert_printf_output_contains("Password inccorect, Enter Your Password again\n2 Trails Left\n");

	// Assert that user was redirected to the Home Page on the second trial
	assert_printf_output_contains("Welcome To Your Home Page");

	// cleanup simulated input
	cleanup_input();

	// Cleanup the captured printf output after the test
	cleanup_printf_output();

	//return DB to its initial state
	universal_cleanup();

}

/************************************************************************************
* Test ID                : BackEnd_CustomerRunner_Login_PasswordIncorrect_SecondTrial
* Description            : Test L: Login to account choice in the Customer Runner function
* Pre-requisites         : Test1_User added to the database
* Test inputs            : User Choice: L (Login)
*                          Username: EdgesAcademy
*                          Password (Trial 1): WrongPassFirstTrial
*                          Password (Trial 2): WrongPassSecondTrial
*                          Password (Trial 3): Edges123
*                          User Choice: O (Logout)
*                          User Choice: Q (Quit)
*                          After returning to Customer Runner: User Choice: Q (Quit)
* Test Expected output   : "Password incorrect, Enter Your Password again. 2 Trials Left"
*                          "Password incorrect, Enter Your Password again. 1 Trial Left"
*                          "Welcome To Your Home Page"
* Reason                 : Password is incorrect on the first try and correct on the second try.
*************************************************************************************/
static void BackEnd_CustomerRunner_Login_PasswordIncorrect_SecondTrial(void)
{
	// Capture the initial user count
	unsigned int initial_user_count = current_user_test;

	// Add a test user to the database
	unsigned char RET = DBM_Add_User(&Test1_User);
	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1);

	// Validate user count is incremented after addition
	CU_ASSERT_EQUAL(current_user_test, initial_user_count + 1);

	// Simulate the sequence of inputs for choice L: Login (Incorrect 2nd trial)
	/*Simulated Inputs:
	 **Customer Runner Page:
	 * User Choice: L (Login)
	 * Username: EdgesAcademy
	 * Password (Trial 1): WrongPassFirstTrial
	 * Password (Trial 2): WrongPassSecondTrial
	 * Password (Trial 3): Edges123
	 **After being redirected to Homepage:
	 * User Choice: O (Logout)
	 * User Choice: Q (Quit)
	 **After returning to Customer Runner Again:
	 User Choice: Q (Quit)
	 */
	simulate_input("L\
                  \nEdgesAcademy\
                  \nWrongPassFirstTrial\
                  \nWrongPassSecondTrial\
                  \nEdges123\
                  \nO\
                  \nQ\
                  \nQ");

	// Redirect printf output to capture printed messages
	redirect_printf_output();

	// Call the function to test
	Customer_Runner();

	// Assert that the password was wrong upon first trial
	assert_printf_output_contains("Password inccorect, Enter Your Password again\n2 Trails Left\n");

	// Assert that the password was wrong upon second trial
	assert_printf_output_contains("Password inccorect, Enter Your Password again\n1Trails Left\n");

	// Assert that user was redirected to the Home Page on the third trial
	assert_printf_output_contains("Welcome To Your Home Page");

	// cleanup simulated input
	cleanup_input();

	// Cleanup the captured printf output after the test
	cleanup_printf_output();

	//return DB to its initial state
	universal_cleanup();

}

/************************************************************************************
* Test ID                : BackEnd_CustomerRunner_Login_PasswordIncorrect_ThirdTrial
* Description            : This test verifies that the system correctly handles the third incorrect password attempt.
* Pre-requisites         : The database contains a valid test user. The user will attempt to log in with incorrect passwords for three trials, 
*                          and the third trial should trigger user deletion.
* Test inputs            : Username: EdgesAcademy
						   Password (Trial 1): WrongPassFirstTrial
						   Password (Trial 2): WrongPassSecondTrial
						   Password (Trial 3): WrongPassThirdTrial
						   User Choice: Q (Quit)
* Test Expected output   : After three incorrect password attempts:
							"Your session is Terminated"
							"User Will Be Deleted, Contact Admin to return it"
							The user count should be decremented by 1 in the database.
* Reason                 : After three failed login attempts, the user should be deleted from the system and the user count is updated accordingly.
************************************************************************************/

static void BackEnd_CustomerRunner_Login_PasswordIncorrect_ThirdTrial(void)
{
	// Capture the initial user count
	unsigned int initial_user_count = current_user_test;
	printf("\nInitial user count: %d\n", current_user_test);
	//DBM_PrintUsers();

	// Add a test user to the database
	unsigned char RET = DBM_Add_User(&Test1_User);
	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1);

	// Validate user count is incremented after addition
	CU_ASSERT_EQUAL(current_user_test, initial_user_count + 1);

	printf("\nUser count after test user was added: %d\n", current_user_test);
	//DBM_PrintUsers();

	// Simulate the sequence of inputs for choice L: Login (Incorrect 3rd trial)
	/*Simulated Inputs:
	 **Customer Runner Page:
	 * User Choice: L (Login)
	 * Username: EdgesAcademy
	 * Password (Trial 1): WrongPassFirstTrial
	 * Password (Trial 2): WrongPassSecondTrial
	 * Password (Trial 3): WrongPassThirdTrial
	 * User Choice: Q (Quit)
	 */
	simulate_input("L\
                  \nEdgesAcademy\
                  \nWrongPassFirstTrial\
                  \nWrongPassSecondTrial\
                  \nWrongPassThirdTrial\
                  \nQ");

	// Redirect printf output to capture printed messages
	redirect_printf_output();

	// Call the function to test
	Customer_Runner();

	// Assert that the password was wrong upon first trial
	assert_printf_output_contains("Password inccorect, Enter Your Password again\n2 Trails Left\n");

	// Assert that the password was wrong upon second trial
	assert_printf_output_contains("Password inccorect, Enter Your Password again\n1Trails Left\n");

	// Assert that the password was wrong upon third trial
	assert_printf_output_contains("Your session is Terminated\nUser Will Be Deleted, Contact Admin to return it\n");


	printf("\nUser count after user was deleted for entering wrong password 3 times: %d\n", current_user_test);
	// Validate user count is decremented as user is deleted (assertion will fail because there is a bug in the DBM_Delete_User Function)
	CU_ASSERT_EQUAL(current_user_test, initial_user_count);

	// cleanup simulated input
	cleanup_input();

	// Cleanup the captured printf output after the test
	cleanup_printf_output();

	//return DB to its initial state
	universal_cleanup();

}

/************************************************************************************
* Test ID                : BackEnd_HomePageRunner_ChangePassword_Successfuly
* Description            : This test verifies that the user can successfully change their password after logging in.
* Pre-requisites         : The database contains a valid user with username "EdgesAcademy" and password "Edges123".
* Test inputs            : User Choice: L (Login)
*                          Username: EdgesAcademy
*                          Password: Edges123
*                          User Choice: C (Change Password)
*                          Current Password: Edges123
*                          New Password: NewPassword
*                          Confirm Password: NewPassword
*                          User Choice: O (Logout)
*                          User Choice: Q (Quit)
* Test Expected output   : "Password Changed Successfully"
* Reason                 : The system allows the user to change their password after verifying the current password and matching the new passwords.
*************************************************************************************/
static void BackEnd_HomePageRunner_ChangePassword_Successfuly(void)
{
	// Capture the initial user count
	unsigned int initial_user_count = current_user_test;

	// Add a test user to the database
	unsigned char RET = DBM_Add_User(&Test1_User);
	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1);

	// Validate user count is incremented after addition
	CU_ASSERT_EQUAL(current_user_test, initial_user_count + 1);

	// Simulate the sequence of inputs for choice C: Change Password (Successful)
	/*Simulated Inputs:
	 **Customer Runner Page:
     * User Choice: L (Login)
     * Username: EdgesAcademy
     * Password: Edges123
     **After being redirected to Homepage::
	 * User Choice: C (Change Password)
	 * Current Password: Edges123
	 * New Password: NewPassword
	 * Confirm Password : NewPassword
	 * User Choice: O (Logout)
	 * User Choice: Q (Quit)
	 */
	simulate_input("L\
                  \nEdgesAcademy\
                  \nEdges123\
		          \nC\
                  \nEdges123\
                  \nNewPassword\
                  \nNewPassword\
                  \nO\
                  \nQ");

	// Redirect printf output to capture printed messages
	redirect_printf_output();

	// To test the Homepage Runner, you have to enter from Customer Runner
	Customer_Runner();

	// Assert that the password changes successfully
	assert_printf_output_contains("Password Changed Successfully \n");

	// cleanup simulated input
	cleanup_input();

	// Cleanup the captured printf output after the test
	cleanup_printf_output();

	//return DB to its initial state
	universal_cleanup();

}

/************************************************************************************
* Test ID                : BackEnd_HomePageRunner_ChangePassword_Mismatch
* Description            : This test verifies that the user cannot change their password if the new password and the confirmation password do not match.
* Pre-requisites         : The database contains a valid user with username "EdgesAcademy" and password "Edges123".
* Test inputs            : User Choice: L (Login)
*                          Username: EdgesAcademy
*                          Password: Edges123
*                          User Choice: C (Change Password)
*                          Current Password: Edges123
*                          New Password: NewPassword
*                          Confirm Password: NewPasswordMismatch
*                          User Choice: O (Logout)
*                          User Choice: Q (Quit)
* Test Expected output   : "New Password mismatched"
* Reason                 : This test is designed to ensure that the system detects and prevents a password change when the new password and confirmation do not match.
*************************************************************************************/

static void BackEnd_HomePageRunner_ChangePassword_Mismatch(void)
{
	// Capture the initial user count
	unsigned int initial_user_count = current_user_test;

	// Add a test user to the database
	unsigned char RET = DBM_Add_User(&Test1_User);
	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1);

	// Validate user count is incremented after addition
	CU_ASSERT_EQUAL(current_user_test, initial_user_count + 1);

	// Simulate the sequence of inputs for choice C: Change Password (Passwords Mismatch)
	/*Simulated Inputs:
	 **Customer Runner Page:
	 * User Choice: L (Login)
	 * Username: EdgesAcademy
	 * Password: Edges123
	 **After being redirected to Homepage::
	 * User Choice: C (Change Password)
	 * Current Password: Edges123
	 * New Password: NewPassword
	 * Confirm Password: NewPasswordMismatch
	 * User Choice: O (Logout)
	 * User Choice: Q (Quit)
	 */
	simulate_input("L\
                  \nEdgesAcademy\
                  \nEdges123\
		          \nC\
                  \nEdges123\
                  \nNewPassword\
                  \nNewPasswordMismatch\
                  \nO\
                  \nQ");

	// Redirect printf output to capture printed messages
	redirect_printf_output();

	// To test the Homepage Runner, you have to enter from Customer Runner
	Customer_Runner();

	// Assert that the password changes successfully
	assert_printf_output_contains("New Password mismatched \n");

	// cleanup simulated input
	cleanup_input();

	// Cleanup the captured printf output after the test
	cleanup_printf_output();

	//return DB to its initial state
	universal_cleanup();

}

/************************************************************************************
* Test ID                : BackEnd_HomePageRunner_ChangePassword_Incorrect
* Description            : This test case checks if the system handles the scenario where the user attempts to change their password but enters an incorrect current password. 
* Pre-requisites         : The database must contain a valid user with username "EdgesAcademy" and password "Edges123".
* Test inputs            : User choice: L (Login)
*                          Username: EdgesAcademy
*                          Password: Edges123
*                          User choice: C (Change Password)
*                          Current Password: WrongPassword
*                          New Password: NewPassword
*                          Confirm Password: NewPassword
*                          User choice: O (Logout)
*                          User choice: Q (Quit).
* Test Expected output   : The system should display the message "Current Password is incorrect".
* Reason                 : To verify that the system correctly handles incorrect current password attempts during a password change operation.
*************************************************************************************/
static void BackEnd_HomePageRunner_ChangePassword_Incorrect(void)
{
	// Capture the initial user count
	unsigned int initial_user_count = current_user_test;

	// Add a test user to the database
	unsigned char RET = DBM_Add_User(&Test1_User);
	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1);

	// Validate user count is incremented after addition
	CU_ASSERT_EQUAL(current_user_test, initial_user_count + 1);

	// Simulate the sequence of inputs for choice C: Change Password (Current Password is Incorrect)
	/*Simulated Inputs:
	 **Customer Runner Page:
	 * User Choice: L (Login)
	 * Username: EdgesAcademy
	 * Password: Edges123
	 **After being redirected to Homepage::
	 * User Choice: C (Change Password)
	 * Current Password: WrongPassword
	 * New Password: NewPassword
	 * Confirm Password : NewPassword
	 * User Choice: O (Logout)
	 * User Choice: Q (Quit)
	 */
	simulate_input("L\
                  \nEdgesAcademy\
                  \nEdges123\
		          \nC\
                  \nWrongPassword\
                  \nNewPassword\
                  \nNewPassword\
                  \nO\
		          \nQ");

	// Redirect printf output to capture printed messages
	redirect_printf_output();

	// To test the Homepage Runner, you have to enter from Customer Runner
	Customer_Runner();

	// Assert that the password changes successfully
	assert_printf_output_contains("Current Password is incorrect \n");

	// cleanup simulated input
	cleanup_input();

	// Cleanup the captured printf output after the test
	cleanup_printf_output();

	//return DB to its initial state
	universal_cleanup();

}

/************************************************************************************
* Test ID                : BackEnd_HomePageRunner_ReserveCourse_NonexistentCourseID
* Description            : This test case checks if the system correctly handles an attempt to reserve a course with a nonexistent course ID. 
* Pre-requisites         : The database must contain a valid user with username "EdgesAcademy" and password "Edges123".
* Test inputs            : User choice: L (Login)
*                          Username: EdgesAcademy
*                          Password: Edges123
*                          User choice: R (Reserve a course)
*                          Course ID: -1 (Nonexistent Course ID)
*                          User choice: O (Logout)
*                          User choice: Q (Quit).
* Test Expected output   : The system should display the message "Out of range Course Try again".
* Reason                 : To verify that the system prevents course reservation attempts with invalid course IDs and provides an appropriate error message.
*************************************************************************************/
static void BackEnd_HomePageRunner_ReserveCourse_NonexistentCourseID(void)
{
	// Capture the initial user count
	unsigned int initial_user_count = current_user_test;

	// Add a test user to the database
	unsigned char RET = DBM_Add_User(&Test1_User);
	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1);

	// Validate user count is incremented after addition
	CU_ASSERT_EQUAL(current_user_test, initial_user_count + 1);

	// Simulate the sequence of inputs for choice R (Reserve a course)
	/*Simulated Inputs:
	 **Customer Runner Page:
	 * User Choice: L (Login)
	 * Username: EdgesAcademy
	 * Password: Edges123
	 **After being redirected to Homepage::
	 * User Choice: R (Reserve a course)
	 * Course ID: -1 (Incorrect Course ID)
	 * User Choice: O (Logout)
	 * User Choice: Q (Quit)
	 */
	simulate_input("L\
                  \nEdgesAcademy\
                  \nEdges123\
		          \nR\
                  \n-1\
				  \nO\
                  \nQ");

	// Redirect printf output to capture printed messages
	redirect_printf_output();

	// To test the Homepage Runner, you have to enter from Customer Runner
	Customer_Runner();

	// Assert that the course ID is wrong
	assert_printf_output_contains("Out of range Course Try again \n");

	// cleanup simulated input
	cleanup_input();

	// Cleanup the captured printf output after the test
	cleanup_printf_output();

	//return DB to its initial state
	universal_cleanup();

}

/************************************************************************************
* Test ID                : BackEnd_HomePageRunner_ReserveCourse_SuccessfulEnrollment
* Description            : This test verifies that a user can successfully enroll in a course by providing valid credentials and a valid course ID.
* Pre-requisites         : A user must be added to the database and be able to log in. The course ID must be valid and available for enrollment.
* Test inputs            : User Choice: L (Login)
*                          Username: EdgesAcademy
*                          Password: Edges123
*                          User Choice: R (Reserve a course)
*                          Course ID: 1 (Valid Course ID)
*                          User Choice: O (Logout)
*                          User Choice: Q (Quit)
* Test Expected output   : The user should be successfully enrolled in the course, with a message confirming the reservation.
* Reason                 : Verifying the correct handling of course reservation with valid inputs and the appropriate system response.
*************************************************************************************/

static void BackEnd_HomePageRunner_ReserveCourse_SuccessfulEnrollment(void)
{
	// Capture the initial user count
	unsigned int initial_user_count = current_user_test;

	// Add a test user to the database
	unsigned char RET = DBM_Add_User(&Test1_User);
	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1);

	// Validate user count is incremented after addition
	CU_ASSERT_EQUAL(current_user_test, initial_user_count + 1);

	// Simulate the sequence of inputs for choice R (Reserve a course)
	/*Simulated Inputs:
	 **Customer Runner Page:
	 * User Choice: L (Login)
	 * Username: EdgesAcademy
	 * Password: Edges123
	 **After being redirected to Homepage::
	 * User Choice: R (Reserve a course)
	 * Course ID: 1 (Correct Course ID)
	 * User Choice: O (Logout)
	 * User Choice: Q (Quit)
	 */
	simulate_input("L\
                  \nEdgesAcademy\
                  \nEdges123\
		          \nR\
                  \n1\
				  \nO\
                  \nQ");

	// Redirect printf output to capture printed messages
	redirect_printf_output();

	// To test the Homepage Runner, you have to enter from Customer Runner
	Customer_Runner();

	// Assert that enrollment is successful
	assert_printf_output_contains("You Are Added To This Course Reservation \n");

	// cleanup simulated input
	cleanup_input();

	// Cleanup the captured printf output after the test
	cleanup_printf_output();

	//return DB to its initial state
	universal_cleanup();

}

/************************************************************************************
* Test ID                : BackEnd_HomePageRunner_ReserveCourse_AlreadyEnrolled
* Description            : This test verifies that a user cannot enroll in a course they are already enrolled in, ensuring proper system validation.
* Pre-requisites         : A user must be added to the database and successfully enrolled in a course.
* Test inputs            : User Choice: L (Login)
*                          Username: EdgesAcademy
*                          Password: Edges123
*                          User Choice: R (Reserve a course)
*                          Course ID: 2 (Course Already Enrolled)
*                          User Choice: O (Logout)
*                          User Choice: Q (Quit)
* Test Expected output   : The user should receive a message indicating they are already enrolled in the course on the second attempt.
* Reason                 : Verifying that the system prevents multiple enrollments in the same course.
*************************************************************************************/

static void BackEnd_HomePageRunner_ReserveCourse_AlreadyEnrolled(void)
{
	// Capture the initial user count
	unsigned int initial_user_count = current_user_test;

	// Add test user 1 to the database
	unsigned char RET = DBM_Add_User(&Test1_User);

	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1);

	// Validate user count is incremented after addition
	CU_ASSERT_EQUAL(current_user_test, initial_user_count + 1);

	// Simulate the sequence of inputs for choice R (Reserve a course)
	/*Simulated Inputs:
	 **Customer Runner Page:
	 * User Choice: L (Login)
	 * Username: EdgesAcademy
	 * Password: Edges123
	 **After being redirected to Homepage::
	 * User Choice: R (Reserve a course)
	 * Course ID: 2 (Correct Course ID Already Enrolled)
	 * User Choice: O (Logout)
	 * User Choice: Q (Quit)
	 */
	simulate_input("L\
                  \nEdgesAcademy\
                  \nEdges123\
		          \nR\
                  \n2\
				  \nO\
                  \nQ");

	// Redirect printf output to capture printed messages
	redirect_printf_output();

	// To test the Homepage Runner, you have to enter from Customer Runner
	Customer_Runner();

	// Assert successful Enrollment
	assert_printf_output_contains("You Are Added To This Course Reservation \n");

	// cleanup simulated input
	cleanup_input();

	// Cleanup the captured printf output after the test
	cleanup_printf_output();

	/* Attempt tp reenroll the same student in the same course again */
	// Simulate the sequence of inputs for choice R (Reserve a course)
     /*Simulated Inputs:
      **Customer Runner Page:
      * User Choice: L (Login)
      * Username: EdgesAcademy
      * Password: Edges123
      **After being redirected to Homepage::
      * User Choice: R (Reserve a course)
      * Course ID: 2 (Correct Course ID Already Enrolled)
      * User Choice: O (Logout)
      * User Choice: Q (Quit)
      */
	simulate_input("L\
                  \nEdgesAcademy\
                  \nEdges123\
		          \nR\
                  \n2\
				  \nO\
                  \nQ");

	// Redirect printf output to capture printed messages
	redirect_printf_output();

	// To test the Homepage Runner, you have to enter from Customer Runner
	Customer_Runner();

	// Assert that student is already enrolled
	assert_printf_output_contains("You Are Already Enrolled to this Course \n");

	// cleanup simulated input
	cleanup_input();

	// Cleanup the captured printf output after the test
	cleanup_printf_output();


	//return DB to its initial state
	universal_cleanup();

}

/************************************************************************************
* Test ID                : BackEnd_HomePageRunner_SeeYourInfo
* Description            : This test case verifies the functionality of the 'See Your Info' option in the HomePage Runner.  
* Pre-requisites         : The test user Test1_User must be added to the database. The system should be able to display of user information correctly.
* Test inputs            : User Choice: L (Login)
						   Username: EdgesAcademy
						   Password: Edges123
						   User Choice: i (see your info)
						   User Choice: O (Logout)
						   User Choice: Q (Quit)
* Test Expected output   : The system should display the user's data after the 'i' option is selected and then log the user out with the message "Good Bye!! see you soon".
* Reason                 : To validate that the system properly displays user information and handles user logout.
*************************************************************************************/
static void BackEnd_HomePageRunner_SeeYourInfo(void)
{
	// Capture the initial user count
	unsigned int initial_user_count = current_user_test;

	// Add a test user to the database
	unsigned char RET = DBM_Add_User(&Test1_User);
	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1);

	// Validate user count is incremented after addition
	CU_ASSERT_EQUAL(current_user_test, initial_user_count + 1);

	// Simulate the sequence of inputs for choice i: see your info
	/*Simulated Inputs:
	 **Customer Runner Page:
	 * User Choice: L (Login)
	 * Username: EdgesAcademy
	 * Password: Edges123
	 **After being redirected to Homepage::
	 * User Choice: i (see your info)
	 * User Choice: O (Logout)
	 * User Choice: Q (Quit)
	 */
	simulate_input("L\
                  \nEdgesAcademy\
                  \nEdges123\
		          \ni\
                  \nO\
                  \nQ");

	// Redirect printf output to capture printed messages
	redirect_printf_output();

	// To test the Homepage Runner, you have to enter from Customer Runner
	Customer_Runner();

	// Assert that the (i) case is entered and DBM_PrintUserData_User is executed
	assert_printf_output_contains("\nThe Data Of User");

	// Assert that the (O) case is executed
	assert_printf_output_contains("Good Bye!! see you soon \nLogged Out\n");

	// cleanup simulated input
	cleanup_input();

	// Cleanup the captured printf output after the test
	cleanup_printf_output();

	//return DB to its initial state
	universal_cleanup();

}

/************************************************************************************
* Test ID                : BackEnd_HomePageRunner_ShowEnrolledCourses
* Description            : This test case verifies the functionality of the 'Show Enrolled Courses' option in the HomePage Runner. 
* Pre-requisites         : The test user Test1_User must be added to the database. The system should be able to display of enrolled courses correctly.
* Test inputs            : User Choice: L (Login)
						   Username: EdgesAcademy
						   Password: Edges123
						   User Choice: R (Reserve a course)
						   Course ID: 1 (Correct Course ID)
						   User Choice: S (Show Enrolled Courses)
						   User Choice: O (Logout)
						   User Choice: Q (Quit)
* Test Expected output   : The system should display the enrolled courses with the message "You Are Currently Enrolled in:", followed by the course details.
* Reason                 : To validate that the system properly displays the list of courses a user is enrolled in after they have successfully reserved a course.
*************************************************************************************/

static void BackEnd_HomePageRunner_ShowEnrolledCourses(void)
{
	// Capture the initial user count
	unsigned int initial_user_count = current_user_test;

	// Add a test user to the database
	unsigned char RET = DBM_Add_User(&Test1_User);
	// Validate addition success
	CU_ASSERT_EQUAL(RET, 1);

	// Validate user count is incremented after addition
	CU_ASSERT_EQUAL(current_user_test, initial_user_count + 1);

	// Simulate the sequence of inputs for choice R (Reserve a course)
	/*Simulated Inputs:
	 **Customer Runner Page:
	 * User Choice: L (Login)
	 * Username: EdgesAcademy
	 * Password: Edges123
	 **After being redirected to Homepage::
	 * User Choice: R (Reserve a course)
	 * Course ID: 1 (Correct Course ID)
	 * User Choice: S (Show Enrolled Courses)
	 * User Choice: O (Logout)
	 * User Choice: Q (Quit)
	 */
	simulate_input("L\
                  \nEdgesAcademy\
                  \nEdges123\
		          \nR\
                  \n1\
                  \nS\
				  \nO\
                  \nQ");

	// Redirect printf output to capture printed messages
	redirect_printf_output();

	// To test the Homepage Runner, you have to enter from Customer Runner
	Customer_Runner();

	// Assert that the (S) case is entered
	assert_printf_output_contains("You Are Currently Enrolled in:\n");

	// cleanup simulated input
	cleanup_input();

	// Cleanup the captured printf output after the test
	cleanup_printf_output();

	//return DB to its initial state
	universal_cleanup();

}

/************************************************************************* App Test Cases ************************************************************************/

/************************************************************************************
* Test ID                : BackEnd_MainAppRunner_AdminVerified
* Description            : This test verifies the correct behavior of the Main App Runner when an Admin is verified with a valid token. 
* Pre-requisites         : The system must be configured with an Admin user (Mohamed Tarek) and a valid token for Admin login.
* Test inputs            : User Type: 0 (Admin)
						   Token: 10203040
						   User Choice: Q (Quit)
* Test Expected output   : The system should display the following:
							1. "Hello Are You Admin Mohamed Tarek?"
							2. "Please Enter Your Secret Token"
							3. "Welcome Mohamed Tarek"
							4. "Welcome To Edges Software"
* Reason                 : To validate that the system properly verifies an Admin's credentials and grants access to the Admin Runner page.
*************************************************************************************/
static void BackEnd_MainAppRunner_AdminVerified(void)
{
	// Simulate the sequence of inputs for Admin (Verified)
	/*Simulated Inputs:
	 **Main App Runner Page:
	 * User Type: 0 (Admin)
	 * Token: 10203040
	 **After being redirected to Admin Runner Page:
	 * User Choice: Q (Quit)
	 */
	simulate_input("0\
                  \n10203040\
                  \nQ");

	// Redirect printf output to capture printed messages
	redirect_printf_output();

	// To test the Homepage Runner, you have to enter from Customer Runner
	Main_App_Runner();

	// Assert the message in Detect_User_Type()
	assert_printf_output_contains("Hello Are You Admin Mohamed Tarek ? \nif yes please Enter 0 \nif No Please Enter 1\nOther Input will close the SW\n");
	// Assert the message in Verify_User()
	assert_printf_output_contains("Hello Admin Mohamed Tarek\nPlease Enter Your Secret Token\n");
	// Assert the message that Admin is Verified
	assert_printf_output_contains("Welcome Mohamed Tarek \n");
	// Assert entered Admin_Runner()
	assert_printf_output_contains("Welcome To Edges Software \n");


	// cleanup simulated input
	cleanup_input();

	// Cleanup the captured printf output after the test
	cleanup_printf_output();

	//return DB to its initial state
	universal_cleanup();

}

/************************************************************************************
* Test ID                : BackEnd_MainAppRunner_AdminNotVerified
* Description            : This test verifies the behavior of the Main App Runner when an Admin user attempts to log in with an incorrect token. 
* Pre-requisites         : The system must be configured with an Admin user (Mohamed Tarek) and invalid tokens for Admin login.
* Test inputs            : User Type: 0 (Admin)
						   Token: 1234567
						   Token: 2397615
						   Token: 0971020
* Test Expected output   : The system should display the following:
							 1. "Hello Are You Admin Mohamed Tarek?"
							 2. "Please Enter Your Secret Token"
							 3. "Wrong Token please Enter Again you have 1 Trial left"
							 4. "You Are a Thief and not Mohamed Tarek\nSoftware Will Close"
* Reason                 : To validate that the system denies access to the Admin when all token attempts are incorrect and closes the software.
*************************************************************************************/

static void BackEnd_MainAppRunner_AdminNotVerified(void)
{
	// Simulate the sequence of inputs for Admin (Not Verified)
	/*Simulated Inputs:
	 **Main App Runner Page:
	 * User Type: 0 (Admin)
	 * Token: 1234567
	 * Token: 2397615
	 * Token: 0971020
	 */
	simulate_input("0\
                  \n1234567\
                  \n2397615\
                  \n0971020");

	// Redirect printf output to capture printed messages
	redirect_printf_output();

	// To test the Homepage Runner, you have to enter from Customer Runner
	Main_App_Runner();

	// Assert the message in Detect_User_Type()
	assert_printf_output_contains("Hello Are You Admin Mohamed Tarek ? \nif yes please Enter 0 \nif No Please Enter 1\nOther Input will close the SW\n");
	// Assert the message in Verify_User()
	assert_printf_output_contains("Hello Admin Mohamed Tarek\nPlease Enter Your Secret Token\n");
	// Assert the message that Admin has 1 trial left
	assert_printf_output_contains("Wrong Token please Enter Again you have 1 Trial left \n");
	// Assert message you are not admin
	assert_printf_output_contains("You Are a Thief and not Mohamed Tarek\nSoftware Will Close\n");


	// cleanup simulated input
	cleanup_input();

	// Cleanup the captured printf output after the test
	cleanup_printf_output();

	//return DB to its initial state
	universal_cleanup();

}

/************************************************************************************
* Test ID                : BackEnd_MainAppRunner_NormalUser
* Description            : This test verifies the behavior of the Main App Runner when a Normal User logs in.
* Pre-requisites         : The system must be configured with a Normal User option.
* Test inputs            : User Type: 1 (Normal User)
* Test Expected output   : The system should display the following:
							 1. "Hello Are You Admin Mohamed Tarek?"
							 2. "if yes please Enter 0" "if No Please Enter 1"
							 4. "Other Input will close the SW"
							 5. "Welcome To Edges Software"
* Reason                 : To verify that the system redirects a Normal User correctly to the Customer Runner page and displays the appropriate messages.
*************************************************************************************/

static void BackEnd_MainAppRunner_NormalUser(void)
{
	// Simulate the sequence of inputs for Normal User
	/*Simulated Inputs:
	 **Main App Runner Page:
	 * User Type: 1 (Normal User)
	 **After being redirected to Customer Runner Page:
	 * User Choice: Q (Quit)
	 */
	simulate_input("1\
                  \nQ");

	// Redirect printf output to capture printed messages
	redirect_printf_output();

	// To test the Homepage Runner, you have to enter from Customer Runner
	Main_App_Runner();

	// Assert the message in Detect_User_Type()
	assert_printf_output_contains("Hello Are You Admin Mohamed Tarek ? \nif yes please Enter 0 \nif No Please Enter 1\nOther Input will close the SW\n");
	// Assert entered Customer_Runner()
	assert_printf_output_contains("Welcome To Edges Software \n");

	// cleanup simulated input
	cleanup_input();

	// Cleanup the captured printf output after the test
	cleanup_printf_output();

	//return DB to its initial state
	universal_cleanup();

}

/************************************************************************************
* Test ID                : BackEnd_MainAppRunner_WrongEntry
* Description            : This test verifies the behavior of the Main App Runner when the user inputs an invalid entry. 
* Pre-requisites         : The system must handle invalid user entries and provide an appropriate response.
* Test inputs            : User Type: 2 (Invalid Entry)
* Test Expected output   : The system should display:
							 1. "Hello Are You Admin Mohamed Tarek?"
							 2. "if yes please Enter 0" "if No Please Enter 1"
							 4. "Other Input will close the SW"
							 5. "Wrong Entry you will quit"
* Reason                 : To verify that the system handles invalid user entries and terminates the program with an error message.
*************************************************************************************/

static void BackEnd_MainAppRunner_WrongEntry(void)
{
	// Simulate the sequence of wrong entry
	/*Simulated Inputs:
	 **Main App Runner Page:
	 * User Type: 2 (Wrong Entry)
	 */
	simulate_input("2");

	// Redirect printf output to capture printed messages
	redirect_printf_output();

	// To test the Homepage Runner, you have to enter from Customer Runner
	Main_App_Runner();

	// Assert the message in Detect_User_Type()
	assert_printf_output_contains("Hello Are You Admin Mohamed Tarek ? \nif yes please Enter 0 \nif No Please Enter 1\nOther Input will close the SW\n");
	// Assert wrong entry
	assert_printf_output_contains("Wrong Entry you will quit\n");

	// cleanup simulated input
	cleanup_input();

	// Cleanup the captured printf output after the test
	cleanup_printf_output();

	//return DB to its initial state
	universal_cleanup();

}

void Main_Test_Runner(void)
{
	/* Initialize the Registry */
	CU_initialize_registry();

	/******************************************************** Creating Test Suites ********************************************************************************/

	/*** Database Test Suites ******************************************************************************************************************/
	CU_pSuite suite_DBM_AddUser = CU_add_suite("Add User Tests", Start_DB_Manager_Tests, End_DB_Manager_Tests);
	CU_pSuite suite_DBM_DeleteUser = CU_add_suite("Delete User Tests", Start_DB_Manager_Tests, End_DB_Manager_Tests);
	CU_pSuite suite_DBM_initDB = CU_add_suite("Initialize DB Tests", Start_DB_Manager_Tests, End_DB_Manager_Tests);
	CU_pSuite suite_DB_AddToCourse = CU_add_suite("Add to Course Tests", Start_DB_Manager_Tests, End_DB_Manager_Tests);
	CU_pSuite suite_DB_DeleteReservation = CU_add_suite("Delete Reservation Tests", Start_DB_Manager_Tests, End_DB_Manager_Tests);

	/*** Create Account Test Suites *************************************************************************************************************/
	CU_pSuite suite_Create_Account_Add = CU_add_suite("Add Account Tests", Start_Create_Account_Tests, End_Create_Account_Tests);
	CU_pSuite suite_Create_Account_Delete = CU_add_suite("Delete Account Tests", Start_Create_Account_Tests, End_Create_Account_Tests);

	/*** Course Registration Test Suites ********************************************************************************************************/
	CU_pSuite suite_Course_Registration = CU_add_suite("Course Registration Tests", Start_Course_Registration_Tests, End_Course_Registration_Tests);

	/*** Login Test Suites **********************************************************************************************************************/
	CU_pSuite suite_Login = CU_add_suite("Login Tests", Start_Login_Tests, End_Login_Tests);

	/*** BackendTest Suites *********************************************************************************************************************/
	CU_pSuite suite_BackEnd = CU_add_suite("BackEnd Tests", Start_Backend_Tests, End_Backend_Tests);

	/*** App Test Suites ************************************************************************************************************************/
	CU_pSuite suite_App = CU_add_suite("App Tests", Start_App_Tests, End_App_Tests);

	/*************************************************** Adding Test Cases To Test Suits*************************************************************************/

	/*** Database Test Cases ******************************************************************************************************************/
	CU_add_test(suite_DBM_AddUser, "Add Valid User", DBM_AddUser_WithValidData);
	CU_add_test(suite_DBM_AddUser, "Add Invalid User", DBM_AddUser_WithInvalidData);
	CU_add_test(suite_DBM_AddUser, "Database Capacity Limit", DBM_AddUser_WhenDatabaseIsFull);
	CU_add_test(suite_DBM_AddUser, "Add User with Duplicate Username", DBM_AddUser_WithDuplicateUsername);

	CU_add_test(suite_DBM_DeleteUser, "Delete Valid User", DBM_DeleteUser_WithValidID);
	CU_add_test(suite_DBM_DeleteUser, "Delete Nonexistent User", DBM_DeleteUser_WithNonExistentID);
	CU_add_test(suite_DBM_DeleteUser, "Delete User from Middle", DBM_DeleteUser_FromMiddleOfDatabase);
	CU_add_test(suite_DBM_DeleteUser, "Delete User with Dependencies", DBM_DeleteUser_WithDependencies);
	CU_add_test(suite_DBM_DeleteUser, "Delete from empty DB", DBM_DeleteUser_FromEmptyDatabase);

	CU_add_test(suite_DBM_initDB, "Initialize DB Valid Data", DBM_initDB_WithValidData);

	CU_add_test(suite_DB_AddToCourse, "Add To Valid Course", DBM_AddToCourse_WithValidData);
	CU_add_test(suite_DB_AddToCourse, "Add To Nonexistent Course", DBM_AddToCourse_NonExistentCourse);
	CU_add_test(suite_DB_AddToCourse, "Add To Course Already Enrolled", DBM_AddToCourse_AlreadyEnrolled);
	CU_add_test(suite_DB_AddToCourse, "Add To Full Course", DBM_AddToCourse_FullCourse);

	CU_add_test(suite_DB_DeleteReservation, "Delete Reservation Valid Data", DBM_DeleteReservation_WithValidData);
	CU_add_test(suite_DB_DeleteReservation, "Delete Unenrolled Student Reservation", DBM_DeleteReservation_StudentNotEnrolled);
	CU_add_test(suite_DB_DeleteReservation, "Delete Reservation Nonexistent Course", DBM_DeleteReservation_NonExistentCourse);
	CU_add_test(suite_DB_DeleteReservation, "Delete Reservation Invalid ID", DBM_DeleteReservation_InvalidStudentID);
	CU_add_test(suite_DB_DeleteReservation, "Delete Reservation No Enrollments", DBM_DeleteReservation_NoEnrollments);

	/*** Create Account Test Cases *************************************************************************************************************/
	CU_add_test(suite_Create_Account_Add, "Add Account Valid User", Add_Account_WithValidData);
	CU_add_test(suite_Create_Account_Add, "Add Account Invalid DOB_Day", Add_Account_With_Invalid_DOB_Day);
	CU_add_test(suite_Create_Account_Add, "Add Account oldest DOB_year", Add_Account_With_Oldest_DOB_Year);
	CU_add_test(suite_Create_Account_Add, "Add Account min name length", Add_Account_With_Min_Name_Length);
	CU_add_test(suite_Create_Account_Add, "Add Account min Password length", Add_Account_With_Min_Password_Length);
	CU_add_test(suite_Create_Account_Add, "Add Account Password Mismatch", Add_Account_PasswordRecheck_Mismatch);
	CU_add_test(suite_Create_Account_Add, "Add Account Age Matches DOB", Add_Account_Age_Matches_DOB);
	CU_add_test(suite_Create_Account_Add, "Add Account to Empty DB", Add_Account_Empty_DB);
	CU_add_test(suite_Create_Account_Add, "Add Account Same Username", Add_Account_Same_Username);

	CU_add_test(suite_Create_Account_Delete, "Delete Account Valid User", Delete_Account_WithValidData);
	CU_add_test(suite_Create_Account_Delete, "Delete Account ID Boundaries", Delete_Account_ID_Boundaries);
	CU_add_test(suite_Create_Account_Delete, "Delete Account ID Out Of Bounds", Delete_Account_ID_OutOfBound);
	CU_add_test(suite_Create_Account_Delete, "Delete Account ID Same User ID", Delete_Account_SameUser);

	/*** Course Registration Test Cases ********************************************************************************************************/
	CU_add_test(suite_Course_Registration, "Register with Valid Data", AddStudentToCourse_WithValidData_And_ReEnrollmentCheck);
	CU_add_test(suite_Course_Registration, "Register Max Capacity", AddStudentToCourse_FullCourseCapacity);
	CU_add_test(suite_Course_Registration, "Register Invalid Course ID", AddStudentToCourse_InvalidCourseID);
	CU_add_test(suite_Course_Registration, "Register Invalid Student ID", AddStudentToCourse_InvalidStudentID);

    /*** Login Test Cases **********************************************************************************************************************/
	CU_add_test(suite_Login, "User Type Admin", Login_Detect_User_Type_Admin);
	CU_add_test(suite_Login, "User Type Normal User", Login_Detect_User_Type_Normal_User);
	CU_add_test(suite_Login, "User Type Incorrect Login", Login_Detect_User_Type_IncorrectLogin);

	CU_add_test(suite_Login, "Admin Correct Token", Login_Verify_Admin_Correct_Token);
	CU_add_test(suite_Login, "Admin 2 Incorrect Token", Login_Verify_Admin_Incorrect_Token_Two_Attempts);
	CU_add_test(suite_Login, "Admin 3 Incorrect Token", Login_Verify_Admin_Incorrect_Token_Three_Attempts);

	CU_add_test(suite_Login, "User Valid Credentials", Login_Verify_User_Valid_Credentials);
	CU_add_test(suite_Login, "User Invalid Pasword", Login_Verify_User_Invalid_Password);
	CU_add_test(suite_Login, "User Nonexistent Username", Login_Verify_User_Nonexistent_Username);

    /*** BackendTest Cases *********************************************************************************************************************/
	CU_add_test(suite_BackEnd, "Admin A: Add User Valid", BackEnd_AdminRunner_AddNewUser_ValidData);
	CU_add_test(suite_BackEnd, "Admin A: Add User Invalid", BackEnd_AdminRunner_AddNewUser_InvalidData);
	CU_add_test(suite_BackEnd, "Admin D: Delete User Valid ID", BackEnd_AdminRunner_DeleteUser_ValidID);
	CU_add_test(suite_BackEnd, "Admin D: Delete User Invalid ID", BackEnd_AdminRunner_DeleteUser_InvalidID);
	CU_add_test(suite_BackEnd, "Admin C: Check Reservation", BackEnd_AdminRunner_CheckReservation);
	CU_add_test(suite_BackEnd, "Admin P: Print All Users", BackEnd_AdminRunner_PrintAllUsers);
	CU_add_test(suite_BackEnd, "Admin U: Print User Exists", BackEnd_AdminRunner_PrintSpecificUser_Exists);
	CU_add_test(suite_BackEnd, "Admin U: Print User Nonexistent", BackEnd_AdminRunner_PrintSpecificUser_Nonexistent);
	CU_add_test(suite_BackEnd, "Admin H: Help", BackEnd_AdminRunner_Help);
	CU_add_test(suite_BackEnd, "Admin Enter Invalid Choice", BackEnd_AdminRunner_InvalidChoice);


	CU_add_test(suite_BackEnd, "Customer C: Create Account Valid", BackEnd_CustomerRunner_CreateNewAccount_ValidData);
	CU_add_test(suite_BackEnd, "Customer C: Create Account Invalid", BackEnd_CustomerRunner_CreateNewAccount_InvalidData);
	CU_add_test(suite_BackEnd, "Customer L: Login Successful", BackEnd_CustomerRunner_Login_Successful);
	CU_add_test(suite_BackEnd, "Customer L: Login User Not Found", BackEnd_CustomerRunner_Login_UsernameNotFound);
	CU_add_test(suite_BackEnd, "Customer L: Login 1 Pass Trial", BackEnd_CustomerRunner_Login_PasswordIncorrect_FirstTrial);
	CU_add_test(suite_BackEnd, "Customer L: Login 2 Pass Trials", BackEnd_CustomerRunner_Login_PasswordIncorrect_SecondTrial);
	CU_add_test(suite_BackEnd, "Customer L: Login 3 Pass Trials", BackEnd_CustomerRunner_Login_PasswordIncorrect_ThirdTrial);

	CU_add_test(suite_BackEnd, "HomePage C: Change Pass Success", BackEnd_HomePageRunner_ChangePassword_Successfuly);
	CU_add_test(suite_BackEnd, "HomePage C: Change Pass Mismatch", BackEnd_HomePageRunner_ChangePassword_Mismatch);
	CU_add_test(suite_BackEnd, "HomePage C: Change Pass Incorrect", BackEnd_HomePageRunner_ChangePassword_Incorrect);
	CU_add_test(suite_BackEnd, "HomePage R: Reserve Course Nonexistent", BackEnd_HomePageRunner_ReserveCourse_NonexistentCourseID);
	CU_add_test(suite_BackEnd, "HomePage R: Reserve Course Successful", BackEnd_HomePageRunner_ReserveCourse_SuccessfulEnrollment);
	CU_add_test(suite_BackEnd, "HomePage R: Reserve Course Already Enrolled", BackEnd_HomePageRunner_ReserveCourse_AlreadyEnrolled);
	CU_add_test(suite_BackEnd, "HomePage i: See your info", BackEnd_HomePageRunner_SeeYourInfo);
	CU_add_test(suite_BackEnd, "HomePage S: Show Enrolled Courses", BackEnd_HomePageRunner_ShowEnrolledCourses);

    /*** App Test Cases ************************************************************************************************************************/
	CU_add_test(suite_App, "Main App Admin Verified", BackEnd_MainAppRunner_AdminVerified);
	CU_add_test(suite_App, "Main App Admin Not Verified", BackEnd_MainAppRunner_AdminNotVerified);
	CU_add_test(suite_App, "Main App Normal User", BackEnd_MainAppRunner_NormalUser);
	CU_add_test(suite_App, "Main App Wrong Entry", BackEnd_MainAppRunner_WrongEntry);

	/* Running the Test Suite through Basic Console */
	//CU_basic_set_mode(CU_BRM_VERBOSE);
	//CU_basic_run_tests();

	/* Running the Test Suite through Console interactive */
	CU_console_run_tests();

	/* Cleanup Registry */
	CU_cleanup_registry();
	
}
