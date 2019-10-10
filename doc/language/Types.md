# Types


## Syntax
```ebnf
type = type_none | type_complete;
type_complete = { modifier } type_primitive | type_function | type_intrinsic;
type_primitive = type_bool | type_int | type_real;
type_function = type parenthesis_left [ COMPLETE_TYPE_LIST ] parenthesis_right;
type_intrinsic = intrinsic_dropmut complete_type | intrinsic_dropref complete_type | intrinsic_type expression;

modifier = modifier_mut | modifier_ref;
```


# Intrinsics


## Type Specifier

The **type!** intrinsic deduces the complete type of an expression. The following hold true:

1. If the `deduced type` of the expression is not the [none type](), the `type` is the `deduced type`.

If the `deduced type` is the [none type](#None%20Type), the program is ill-formed.


## Drop Mutable Specifier

The **dropmut!** intrinsic will discard the mutable specifier of the given complete `type`. The following hold true:

1. If `type` is neither a [mutable type](#Mutable%20Type) nor the [none type](#None%20Type), the resultant type is `type`.
2. If `type` is a [mutable type](#Mutable%20Type), the resultant type is the *mut-unqualified* transformation of `type`.

If `type` is the [none type](#None%20Type), the program is ill-formed.

There is no support for "mutable collapsing" (syntactically implied). To ensure a type is *mut-qualified* while using the `type!` intrinsic, the `dropmut!` modifier must be used.


## Drop Reference Specifier

The **dropref!** intrinsic will discard the reference specifier of the given complete `type`. The following hold true:

1. If `type` is neither a [reference type](#Reference%20Type) nor the [none type](#None%20Type), the resultant type is `type`.
2. If `type` is a [reference type](#Reference%20Type), the resultant type is the *ref-unqualified* transformation of `type`.

If `type` is the [none type](#None%20Type), the program is ill-formed.

There is no support for "reference collapsing" (syntactically implied). To ensure a type is *ref-qualified* while using the `type!` intrinsic, the `dropref!` modifier must be used.


# Type Concepts


## Complete Type

A **complete type** is any `type` that is not the [none type](#None%20Type).


## Base Type

The **base type** of a type is the resultant type after modifiers are stripped. The following hold true:

1. The base type of the [none type](#None%20Type) is `type` (syntactically implied).
2. The base type of a [mutable type](#Mutable%20Type) is the base type of `dropmut! type`.
3. The base type of a [reference type](#Reference%20Type) is the base type of `dropref! type`.
4. The base type of a [function type](#Function%20Type) is the same function type (syntactically implied).
5. The base type of an unqualified non-function type is `type`.

Therefore, the base type of any given `type` is equivilent to `dropmut! dropref! type` such that `type` is not the [none type](#None%20Type).


## Mutable Type

All sound entities are immutable by default unless their type is `mut-qualified`. To `mut-qualify` a type, the mutable modifier (`mut`) must precede the type name. A mutable type is sound if all of the following criteria are met:

1. The [base type](#Base%20Type) of `type` is not the [none type](#None%20Type) (syntactically implied).
2. The `type` has the mutable modifier (`mut`) (syntactically implied).
3. The `type` is not a [function type](#Function%20Type) (syntactically implied).


## Reference Type

All sound entities are value types by default unless their type is `ref-qualified`. To `ref-qualify` a type, the reference modifier (`&`) must precede the type name. A reference type is sound if all of the following criteria are met:

1. The [base type](#Base%20Type) of `type` is not the [none type](#None%20Type) (syntactically implied).
2. The `type` has the reference modifier (`&`) (syntactically implied).
3. The `type` is not a [function type](#Function%20Type) (syntactically implied).


# Language Types


## None Type

The **none type** is the only non-complete type and may only be present as a return type. A none type is sound if all of the following criteria are met:

1. The [base type](#Base%20Type) of `type` is *none* (syntactically implied).
2. The `type` has no modifiers (syntactically implied).

If the unqualified transformation of any given `type` is *none*, `type` must be the none type or the program is ill-formed.


## Primitive Type

A **primitive type** is a *ref-unqualified* built-in type. A primitive type is sound if all of the following criteria are met:

1. The [base type](#Base%20Type) of `type` is one of `{ bool, int, real }` (syntactically implied).
2. The `type` is not a [reference type](#Reference%20Type) (semantically implied).


## Function Type

A **function type** is a complete type that refers to a function. A function type is sound if all of the following criteria are met:

1. The [base type](#Base%20Type) of `type` is not the [none type](#None%20Type) (syntactically implied).
2. The `type` is not a [reference type](#Reference%20Type) (syntactically implied).
3. The `type` is not a [mutable type](#Mutable%20Type) (syntactically implied).
4. The `return type` is any sound type (syntactically implied).
5. The `parameter types` is empty or are all sound complete types (syntactically implied).
