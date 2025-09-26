#include <stdint.h>

#include <bitset>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <stack>
#include <limits>

using namespace std;

/*
 * *** STUDENTS WILL NEED TO CHANGE INPUT_CSV_FILE PATH BELOW TO POINT TO THE rpn-input.csv FILE ***
 * *** ON THEIR LAPTOP/COMPUTER ***
 */
#define INPUT_CSV_FILE "C:/Users/josep/OneDrive/Desktop/Github/Thao-Trans-RPN-Bitwise-Calculator/rpn-input.csv"

// test controls
#define MIN_VALUE 0
#define MAX_VALUE 100
uint8_t const table_width[] = {14, 18, 14, 18, 14, 18};

// test harness structs and params
#define VALUE_NULLPTR -999

enum command : uint16_t
{
    cmd_enter = 0,
    cmd_clear,
    cmd_pop,
    cmd_top,
    cmd_left_shift,
    cmd_right_shift,
    cmd_or,
    cmd_and,
    cmd_add,
};
vector<string> command_name = {"cmd_enter", "cmd_clear", "cmd_pop", "cmd_top", "cmd_left_shift",
                               "cmd_right_shift", "cmd_or", "cmd_and", "cmd_add"};
uint8_t const width = 16U;

// stack used for operations
stack<uint16_t> st;

// adds two binary numbers and adds it to stack
// returns nullptr if overflow occurs
shared_ptr<uint16_t> add(uint16_t a, uint16_t b)
{
    if (b > numeric_limits<uint16_t>::max() - a)
    {
        st.push(b);
        st.push(a);
        return nullptr;
    }

    while (b != 0)
    {
        uint16_t carry = a & b;
        a = a ^ b;
        b = carry << 1;
    }
    st.push(a);
    return make_shared<uint16_t>(st.top());
}

// These operators follow similar solutions so I made
// a switch for them to reduce repeating code
shared_ptr<uint16_t> commands(int cmd)
{
    if (st.size() < 2)
    {
        return nullptr;
    }
    uint16_t a = st.top();
    st.pop();
    uint16_t b = st.top();
    st.pop();
    switch (cmd)
    {
    case 0:
        st.push(b << a);
        break;
    case 1:
        st.push(b >> a);
        break;
    case 2:
        st.push(a | b);
        break;
    case 3:
        st.push(a & b);
        break;
    case 4:
        return add(a, b);
    default:
        break;
    }
    return make_shared<uint16_t>(st.top());
}

// These operators follow similar solutions so I made
// a switch for them to reduce repeating code
shared_ptr<uint16_t> empty_stack(int cmd)
{
    if (st.empty())
    {
        return nullptr;
    }
    switch (cmd)
    {
    case 0:
        st.pop();
        if (st.empty())
        {
            return nullptr;
        }
        break;
    case 1:
        break;
    default:
        break;
    }
    return make_shared<uint16_t>(st.top());
}

shared_ptr<uint16_t> rpn_calc(command const cmd, uint16_t const value = 0)
{
    uint16_t val = value;
    shared_ptr<uint16_t> result = nullptr;
    // switch to execute respective commands
    switch (cmd)
    {
    case cmd_enter:
        st.push(val);
        result = make_shared<uint16_t>(st.top());
        break;
    case cmd_clear:
        while (!st.empty())
        {
            st.pop();
        }
        result = nullptr;
        break;
    case cmd_pop:
        result = empty_stack(0);
        break;
    case cmd_left_shift:
        result = commands(0);
        break;
    case cmd_right_shift:
        result = commands(1);
        break;
    case cmd_top:
        result = empty_stack(1);
        break;
    case cmd_or:
        result = commands(2);
        break;
    case cmd_and:
        result = commands(3);
        break;
    case cmd_add:
        result = commands(4);
        break;
    default:
        break;
    }

    return result;
}

/*
 * *** STUDENTS SHOULD NOT NEED TO CHANGE THE CODE BELOW. IT IS A CUSTOM TEST HARNESS. ***
 */

void header()
{
    cout << left << setw(table_width[0]) << setfill(' ') << "pass/fail";
    cout << left << setw(table_width[1]) << setfill(' ') << "command";
    cout << left << setw(table_width[2]) << setfill(' ') << "value";
    cout << left << setw(table_width[3]) << setfill(' ') << "value bits";
    cout << left << setw(table_width[4]) << setfill(' ') << "result";
    cout << left << setw(table_width[5]) << setfill(' ') << "result bits" << endl;

    cout << left << setw(table_width[0]) << setfill(' ') << "--------";
    cout << left << setw(table_width[1]) << setfill(' ') << "--------";
    cout << left << setw(table_width[2]) << setfill(' ') << "--------";
    cout << left << setw(table_width[3]) << setfill(' ') << "--------";
    cout << left << setw(table_width[4]) << setfill(' ') << "--------";
    cout << left << setw(table_width[5]) << setfill(' ') << "--------" << endl;
}

void print_row(bool const test_success, command const cmd, int16_t const value, shared_ptr<uint16_t> top_of_stack)
{
    // print results
    string const pass_fail = test_success ? "PASS" : "FAIL";
    cout << left << setw(table_width[0]) << setfill(' ') << pass_fail;
    cout << left << setw(table_width[1]) << setfill(' ') << command_name[cmd];
    if (value == VALUE_NULLPTR)
    {
        cout << left << setw(table_width[2]) << setfill(' ') << " ";
        cout << left << setw(table_width[3]) << setfill(' ') << " ";
    }
    else
    {
        cout << left << setw(table_width[2]) << setfill(' ') << value;
        cout << left << setw(table_width[3]) << setfill(' ') << bitset<width>(value);
    }

    if (top_of_stack)
    {
        cout << left << setw(table_width[4]) << setfill(' ') << *top_of_stack;
        cout << left << setw(table_width[5]) << setfill(' ') << bitset<width>(*top_of_stack) << endl;
    }
    else
    {
        cout << left << setw(table_width[4]) << setfill(' ') << " ";
        cout << left << setw(table_width[5]) << setfill(' ') << " " << endl;
    }
}

vector<string> split(string const &s, string const &delimiter)
{
    vector<string> tokens;
    size_t pos = 0;
    size_t start = 0;
    string token;
    while (pos != string::npos)
    {
        pos = s.find(",", start);
        token = s.substr(start, pos - start);
        tokens.push_back(token);
        start = pos + delimiter.length();
    }

    return tokens;
}

void init_command_map(unordered_map<string, command> &command_map)
{
    for (size_t i = 0; i < command_name.size(); i++)
    {
        string const cmd = command_name[i];
        command_map[cmd] = static_cast<command>(i);
    }
}

bool parse_csv_line(string const line, unordered_map<string, command> command_map, command &input_cmd,
                    uint16_t &input_value, int32_t &answer_value)
{
    try
    {
        vector<string> tokens = split(line, ",");

        // get command
        string cmd = tokens[0];
        // if command is  valid
        if (command_map.count(cmd) == 1)
        {
            input_cmd = command_map[cmd];
        }
        else
        {
            cout << "ERROR: Invalid command cmd=" << cmd << endl;
            return false;
        }

        // get input and value
        input_value = stoi(tokens[1].c_str());
        answer_value = stoi(tokens[2].c_str());
    }
    catch (exception const &e)
    {
        cout << "ERROR: Unable to parse input csv file, line=" << line << endl;
        cout << "ERROR: exception e=" << e.what() << endl;
        return false;
    }

    return true;
}

bool test()
{
    // open input file
    ifstream input_file(INPUT_CSV_FILE);
    if (!input_file.is_open())
    {
        cout << "ERROR: Unable to find and open the file " << INPUT_CSV_FILE << endl;
        cout << "       Make sure the path to the file is correct in your code" << endl;
        return false;
    }

    // init command map
    unordered_map<string, command> command_map;
    init_command_map(command_map);

    // print header
    header();

    bool success = true;
    bool all_test_pass = true;
    uint16_t pass = 0;
    string line;
    size_t row = 0;
    while (getline(input_file, line))
    {
        // cout << "line " << row << ":" << line << endl;
        if (row > 0)
        {
            // parse csv line
            command input_cmd;
            uint16_t input_value;
            int32_t input_answer;
            bool parse_success = parse_csv_line(line, command_map, input_cmd, input_value, input_answer);
            if (!parse_success)
            {
                return false;
            }

            // set answer value
            shared_ptr<uint16_t> answer;
            if (input_answer == VALUE_NULLPTR)
            {
                answer = nullptr;
            }
            else
            {
                answer = make_shared<uint16_t>(input_answer);
            }

            // do the command
            shared_ptr<uint16_t> result = rpn_calc(input_cmd, input_value);

            // test the results
            bool test_success = false;
            bool both_null = answer == nullptr && result == nullptr;
            bool both_same_value = answer && result && (*answer == *result);
            if (both_null || both_same_value)
            {
                pass += 1;
                test_success = true;
            }

            // update all tests
            all_test_pass = all_test_pass && test_success;

            // print row
            print_row(test_success, input_cmd, input_value, result);
        }
        row += 1;
    }

    // close input file
    input_file.close();

    // summarize results
    cout << "-------------------------------------------" << endl;
    if (all_test_pass)
    {
        cout << "SUCCESS ";
    }
    else
    {
        cout << "FAILURE ";
    }
    const size_t num_tests = row - 1;
    cout << pass << "/" << num_tests << " passed" << endl;
    cout << "-------------------------------------------" << endl;

    return success;
}

int main()
{
    if (!test())
    {
        return -1;
    }
    return 0;
}