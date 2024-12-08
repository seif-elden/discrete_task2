## ReadMe: Explanation and Dry Run for the Logic Circuit Analysis Program

### Overview
This C++ program analyzes logical circuit expressions, compares their behavior, and evaluates properties such as equivalence, satisfiability, and tautology. The program takes two logical expressions in **infix notation** (e.g., `(A & B) | (!C)`), converts them into **postfix notation** (e.g., `AB&C!|`), and processes them.

Key functionalities include:
1. **Infix to Postfix Conversion:** Converts user-friendly infix expressions to postfix for easier evaluation.
2. **Truth Table Generation:** Generates a truth table for each circuit.
3. **Equivalence Check:** Determines if the two circuits produce identical outputs for all inputs.
4. **Satisfiability and Tautology Check:** Analyzes if a circuit is always true (tautology), never true (unsatisfiable), or satisfiable.
5. **Circuit Modification:** Modifies unsatisfiable or tautological circuits to make them satisfiable.

---

### Functions

#### 1. `infixToPostfix(const string& infix)`
This function converts infix expressions (human-readable format) to postfix notation (easy-to-evaluate format). It uses the **Shunting Yard Algorithm**:
- Operators (`|`, `&`, `!`) have precedence:
  - `!` (NOT) has the highest precedence.
  - `&` (AND) has medium precedence.
  - `|` (OR) has the lowest precedence.
- Parentheses are used to control operator precedence.
- Operands (variables like `A`, `B`, `C`) are directly added to the postfix output.

### Dry run for clarity and accuracy.

**input:**
```plaintext
(A & B) | (!C)
```

---

#### Initialization:
- **Precedence Map:**
  ```
  {'|': 1, '&': 2, '!': 3}
  ```
- **Operators Stack:** Empty at the start: `[]`
- **Postfix String:** Empty at the start: `""`

---

#### Step-by-Step Execution:

| **Step** | **Character** | **Action**                                                                 | **Operators Stack**   | **Postfix String** |
|----------|---------------|---------------------------------------------------------------------------|-----------------------|--------------------|
| 1        | `(`           | Push `(` onto the stack                                                  | `[(]`              | `""`              |
| 2        | `A`           | Append operand `A` to the postfix string                                 | `[(]`              | `"A"`             |
| 3        | `&`           | Push `&` onto the stack (higher precedence than `(`)                     | `[(,&]`          | `"A"`             |
| 4        | `B`           | Append operand `B` to the postfix string                                 | `[(,&]`          | `"AB"`            |
| 5        | `)`           | Pop and append operators until `(` is encountered, then discard `(`     | `[]`                 | `"AB&"`           |
| 6        | `\|`           | Push `\|` onto the stack                                                  | `[\|]`              | `"AB&"`           |
| 7        | `(`           | Push `(` onto the stack                                                  | `[\|,(]`           | `"AB&"`           |
| 8        | `!`           | Push `!` onto the stack (highest precedence)                             | `[\|,(,!]`       | `"AB&"`           |
| 9        | `C`           | Append operand `C` to the postfix string                                 | `[\|,(,!]`       | `"AB&C"`          |
| 10       | `)`           | Pop and append operators until `(` is encountered, then discard `(`     | `[\|]`              | `"AB&C!"`         |
| 11       | End of input  | Pop all remaining operators from the stack and append to the postfix     | `[]`                 | `"AB&C!\|"`        |

---

#### Final Output:
**Postfix Expression:**  
```plaintext
AB&C!|
```

---


#### Verification:
**Infix Input:**  
```plaintext
(A & B) | (!C)
```

**Postfix Output:**  
```plaintext
AB&C!|
```  

The corrected dry run confirms the accurate conversion of the infix expression to postfix.

---




#### 2. `evaluateExpression(string expr, const map<char, bool>& vars)`
This function evaluates a postfix logical expression using a stack. It supports:
- Variables (`A`, `B`, `C`) replaced by boolean values (`true` or `false`).
- Operators:
  - `!` (NOT)
  - `&` (AND)
  - `|` (OR)

**Steps:**
1. Traverse the postfix expression:
   - If it's a variable, push its value (from the input map) onto the stack.
   - For `!` (NOT), pop the top of the stack, apply NOT, and push the result.
   - For `&` or `|`, pop the top two values, apply the operator, and push the result.
2. At the end, the stack contains the result of the expression.

**Example:**
Let's perform a **dry run** for the `evaluateExpression` function with the following input:

### Input:
- **Postfix Expression:** `AB&C!|`
- **Variable Values (Map):**
  ```
  A = true, B = false, C = true
  ```

---

#### Initialization:
- **Stack:** Empty at the start: `[]`

---

#### Step-by-Step Execution:

| **Step** | **Character** | **Action**                               | **Stack (After Action)** |
|----------|---------------|------------------------------------------|--------------------------|
| 1        | `A`           | Push value of `A` (`true`) onto stack    | `[true]`                 |
| 2        | `B`           | Push value of `B` (`false`) onto stack   | `[true, false]`          |
| 3        | `&`           | Pop two values (`true`, `false`), apply AND (`true && false = false`), push result | `[false]`                |
| 4        | `C`           | Push value of `C` (`true`) onto stack    | `[false, true]`          |
| 5        | `!`           | Pop one value (`true`), apply NOT (`!true = false`), push result | `[false,false]`                |
| 6        | `\|`           | Pop two values (`false`, `false`), apply OR (`false \| false = false`), push result | `[false]`                 |

---

#### Final State:
- **Stack:** `[false]`
- **Return Value:** `false`

---


#### Verification:
- **Postfix Expression:** `AB&C!|`
- **Variable Values:** `A = true, B = false, C = true`
- **Evaluation Result:** `false`

The dry run confirms that the function correctly evaluates the logical expression.

---


#### 3. `generateInputs()`
Generates all possible combinations of 3 boolean variables (`A`, `B`, `C`):
- Since there are 3 variables, there are \(2^3 = 8\) combinations.

**Output:** A vector of maps where each map corresponds to one combination:
- Example:
  - Combination 1: `{A: false, B: false, C: false}`
  - Combination 2: `{A: false, B: false, C: true}`
  - ...

---



#### 4. `analyzeCircuits(string circuit1, string circuit2)`
The main function that:
1. Converts infix expressions to postfix using `infixToPostfix`.
2. Generates all input combinations using `generateInputs`.
3. Computes the truth table for both circuits using `evaluateExpression`.
4. Checks equivalence:
   - Compares the output vectors of both circuits.
5. Evaluates satisfiability and tautology for each circuit:
   - **Satisfiable:** At least one output is `true`.
   - **Tautology:** All outputs are `true`.
6. Modifies unsatisfiable or tautological circuits by changing an operator (`&` ↔ `|`).



---


### Dry Run of the task

#### Input:
```plaintext
infix1 = "(A & B) | (!C)"
infix2 = "( ( (A & B) | (!C) ) & ( (A & B) | (!C) ) ) | !( ( (A & B) | (!C) ) & ( (A & B) | (!C) ) )"
```

#### Conversion:
- Circuit 1 (Postfix): `AB&C!|`
- Circuit 2 (Postfix): `AB&C|AB&C|&AB&C|AB&C|&!|`

#### Truth Table:

**Circuit 1:**

```
A B C | Result
--------------
0 0 0 | 1
0 0 1 | 0
0 1 0 | 1
0 1 1 | 0
1 0 0 | 1
1 0 1 | 1
1 1 0 | 1
1 1 1 | 1
```

**Circuit 2:**

```
A B C | Result
--------------
0 0 0 | 1
0 0 1 | 0
0 1 0 | 1
0 1 1 | 0
1 0 0 | 1
1 0 1 | 1
1 1 0 | 1
1 1 1 | 1
```

#### Results:
- **Equivalence:** Yes (identical truth tables).
- **Satisfiability:** Both circuits are satisfiable.
- **Tautology:** Neither circuit is a tautology.

---


#### other example:

```
Converted Circuit 1 to Postfix: AA!|
Converted Circuit 2 to Postfix: ABC&!|

Truth Table for Circuit 1: AA!|
A B C | Result
----------------
0 0 0 | 1
0 0 1 | 1
0 1 0 | 1
0 1 1 | 1
1 0 0 | 1
1 0 1 | 1
1 1 0 | 1
1 1 1 | 1

Truth Table for Circuit 2: ABC&!|
A B C | Result
----------------
0 0 0 | 1
0 0 1 | 1
0 1 0 | 1
0 1 1 | 0
1 0 0 | 1
1 0 1 | 1
1 1 0 | 1
1 1 1 | 1

The two circuits are not equivalent.

Circuit 1 is: Tautology
Circuit 2 is: Satisfiable
Modified Circuit 1 

```
