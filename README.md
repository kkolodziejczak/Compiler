# Compiler

My compiler for MIPS CPU written at university. My language is very similar to C language.

## To develop

Make sure that you have downloaded/installed following programs:

- Mars - [MIPS CPU simulator](http://courses.missouristate.edu/KenVollmar/mars/download.htm)
- Bison - Parser generator ``$ sudo apt-get install Bison ``
- Flex - Lexical Analyzer ``$ sudo apt-get install Flex ``

### Compile compiler
To compile compiler simply run MakeFile
``$ make``

### Compile program
To compile your program type into console 
    ``$ ./leks "FileWithSourceCode" "OutputFileName"``
If source code didn't have any errors, file with "OutputFileName" will be generated with all assembly code.

### Run compiled program
To run compiled program start [Mars](http://courses.missouristate.edu/KenVollmar/mars/download.htm) and open file with assembly code `` File -> Open `` or create new ``File -> New `` and Copy and Paste assembly code into blank .asm file.
After that you need to save .asm file with all that done you can Assemble it `` Run -> Assemble `` or simply press F3 key to run it go to `` Run -> Go `` or press F5 key.

There is also an option to run your program from command line with following command

``$ java -jar "PathToYoutMarsProgram" "FileWithAssemblyCode" ``

--------------
# Language documentation

### Data types Int and Float
- With variable declaration we are required to define their type,
- There is an option to declare variable without assigning value.
```C
int main(){
  int Digit;
  int i = 0;
  float DecimalValue;
  float f = 1.0;
}
```
### Arithmetic expressions
- In case of working with different datatypes they will be converted into more general type, __the decimal value will be cut off and not rounded.__
- In expressions we can use following arithmetic operations:
  - Add `+`
  - Subtract `-`
  - Multiply `*`
  - Division `/`
  - Brackets `(` `)`
```C
int main(){
    int a = 3;
    float b = 12.2;
    println(a+b*2-1);
    println((a+5)*4/2);
    println(a+5*4/2);
}
```
### Conditional operation IF
- Instruction can be nested,
- The instructions will be taken into account when placed in braces `{` `}`, if they are missing, the first line of code will be taken into account.
```C
int main(){
    int a;
    in(a);
    if(a == 1){
        println("Yes, a == 1");
    }
}
```
### Input from Keyboard, IN Function
- There is an possibility to read `Ints` and `Floats` into variables,
- It is not possible to read `Ints` or `Floats` into the array. 
```C
int main(){
    float a;
    print("Write decimal number: ");
    in(a);
    println("");
    println(a);

    if(a == 1.0)
        println("Yes, a == 1.0");
    else
        println("No 1 != 1.0!");
}
```
### Printing on screen, PRINT and PRINTLN Functions
- It is possible to print `Ints`, `Floats` and `String` values.
- In contrast, as with `IN`, it is possible to display values from an array.
```C
int main(){
    float a;
    print("Write decimal number: ");
    in(a);
    println("");
    println(a);

    int b;
    print("Write integer number: ");
    in(b);
    println("");
    println(b);
}
```

### While loop
  - While loop may have a condition written with expressions on both sides of the conditional character,
  - The instructions will be taken into account when placed in braces `{` `}`, if they are missing, the first line of code will be taken into account.
```C
int main(){
    int i = 0;
    while(i < 10){
        println(i);
        i = i + 1;
    };
}
```
### String type
 - String type allows us to display a string on the screen,
 - In the presented language there is no possiblity to create varialbe of string type,
```C
int main(){
  println("An example string that can be displayed!");
  print("Special symbols can be used '\\t' \t oraz '\\n'\n . <====");
}
```
### One-dimensional arrays
  - The declaration should be without attribution,
  - The size of the array we need to specify integers,
  - References to an array can be used by numbers or expressions.
```C
int main(){
    int a[10];
    a[0] = 12;
    println(a[0]);
    int c = 0;
    println(a[c]);
}
```
### A complex conditional statement with an alternative
  - Instruction can be nested,
  - The instructions will be taken into account when placed in braces `{` `}`, if they are missing, the first line of code will be taken into account.
```C
int main(){
  int a;
  println("Write integer value");
  in(a);
  if(a > 0){
      if(a + 2 == 4){
          print("value + 2 == 4\n");
      }
      else
          print("value + 2 != 4\n");
  }else{
    if(a+4 < 0){
      if(a < -10){
        if(a == -15)
          println("value == -15");
        else
          println("Value is greater than -10");
      }
    }else{
      println("Value is in between 0,-4");
    }
  }
}
```

### Multidimensional arrays
 - Multidimensional arrays can be created of any size,
 - We can refer to individual memory cells by using expressions,
 - It is possible to display a cell from an array,
 - __Could not load value from keypad directly to memory cell.__
```C
int main(){
  int a[10,10];
  int i =0;
  while(i < 10){
    a[i,i] = i + a[i,i];
    i++;
  }
}
```
### Functions without parameters that return an int / float value
  - After the function call, __we need to take care of returned value!__,
  - The instructions will be taken into account when placed in braces `{` `}`, if they are missing, the first line of code will be taken into account.
```C
int function_name_i(){
  //function body
  return 0;
}
float function_name_f()
  return 1.23;

int main(){
    int result = function_name_i();
    println(result);
    println(function_name_f());
}
```
### Pre / Post increment, decrement for factors
  - Post-increment `++` and decrement `--` operations can be in a single line without assignment,
  - __Can not use in expressions!__
```C
int main(){
  int a = 0;
  a++;
  int c = 23 * 2 + a;
  println(c);
}
```
### Calculating the value of the rest of the divide 
  - We have a `%` operator that allows us to calculate the remainder of the divide.
```C
int main(){
  int number;
  in(number);

  if(number % 2 == 0){
    println("Your nubmer is even!");
  }else
    println("Your number is odd!");
}
```
