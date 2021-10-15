## Code Structure
Contains
  - Two main classes (Parser, Display)
  - Five directories (common, Parser, Display, Driver, Tests)
  - Uses CMake build system to generate static libraries which can be consumed by external applications.
  - Uses CTest(Cmake provided Unit test framework)
  - Proper error checking using return values.
  - Uses boost Librries (program_options, Tokenizer, PropertyTree, FileSystem)
  - Proper Exception Handling.
    - Code catches exceptions defined for Boost, STLs, inbuilt functions.
    - Defined "class NULLException" for catching NULL Pointer Exception.

## Directories and classes
### 1. common
- (common.h) which contains
  - type aliases
  - Preprocessor directives
- (null_pointer_exception.h) which contains "class NULLException" which catches NULL Pointer Exception
- (exceptions.h) which contains commonly thrown exceptions
### 2. Parser 
Contains Parser class. This class parses the input.txt file and reads (smiley indexes and top x words) and stores in `vector<int>` and `vector<pair<string,int>>`
- Parse class uses:
  - boost filesystem to open and check input file.
  - boost tokenizer to parse tokens seperated by token(`" " | '\r'`)
### 3. Display: 
Contains Display class. This class is responsible for dumping data to different formats:
- Display class can dump data to any of following outputs. It utilizes stringstream.
  - console
  - text file
  - json file
  - xml file
- This class uses:
  - boost program_options library to create elegent command line. User can dump the data into either or all of above mentioned files
  - boost PropertyTree for creating xml and json files
### 4. Driver
This is external interface utilizing the Parser and Display libraries.
  - Creates Display Object and notes input provided by user on command line (output redirections).
  - Creates Parser object to parse input file
  - Call Dump() method from Display class to dump information on various outputs asked by user.
### 5. Tests: Contains CmakeTests. Unit tests are written here
  - I have explicity written test case to abort in case of failure, so that nothing goes un-noticed.

## Output Formats
1. Created xml
```c
<output>
    <top_words>
        <word>word1</word>
        <word>word2</word>
        <word>wordn</word>
    </top_words>
    <indexes_of_smileys>
        <index>index1</index>
        <index>index2</index>
        <index>indexn</index>
    </indexes_of_smileys>
</output>
```
2. Json
```json
{
    "top_words":
    {
        "word1": count-of-word1,
        "word2": count-of-word2,
        "wordn": count-of-wordn
    },
    "index_of_smileys": [
        index1,
        index2,
        indexn
    ],
}  
```
3. console
```c
Top X used Words
Word : Count
your : 7
you : 7
towards : 7
you. : 4
it : 4
move : 3
character : 3
not : 3
moves : 3
1 : 3

Indexes of Smileys
0 1 2 18
```
4. Text file
```c
Top X used Words
Word : Count
your : 7
you : 7
towards : 7
you. : 4
it : 4
move : 3
character : 3
not : 3
moves : 3
1 : 3

Indexes of Smileys
0 1 2 18  
```
