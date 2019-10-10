# Definition


## Syntax

```ebnf
variable_definition = keyword_let DECLARATION [ assign expression ] semicolon;
function_definition = keyword_fn identifier
    parenthesis_left [ DECLARATION_LIST ] parenthesis_right colon type [ STATEMENT_LIST ] keyword_end;
```


## Variables

A **variable declaration** declares a variable by specifying an [identifier]() and a [type](). A variable declaration is sound if all of the following criteria are met:

1. The [base type]() of `type` is not *none* (syntactically implied).
2. If `type` is a [reference type](), the criteria of reference type are met (syntactically implied).
3. If `type` is a [function type](), the critera of function type are met (syntactically implied).

A **variable definition** is a *variable declaration* that is preceded by `let` and optionally initializes the variable with an [expression](). A variable definition is sound if all of the following criteria are met:

1. The *variable declaration* is sound (syntactically implied).
2. If `type` is a [reference type](), the *variable definition* must be explicitly-initialized to a compatible entity and the mutability is compatible.
3. If `type` is a [function type](), the *variable definition* must be explicitly-initialized to a compatible entity.
4. If `type` is neither a reference type nor a function type, the *variable definition* must either be explicitly-initialized to a compatible entity or can be [default initializated]().


## Functions

A **function definition** defines a function by specifying an [identifier](), a [return type](), a list of 0 or more parameters ([declarations]()), and a list of 0 or more [statements](). A function definition is sound if all of the following critera are met:

1. The `type` is sound (syntactically implied).
2. If the *type* of the `return type` is a *complete type*, the function returns an entity of type `return type`.
3. If the *type* of the `return type` is *none*, the function must not return an entity.

The *none* type is not a *complete_type*, therefore no modifiers
