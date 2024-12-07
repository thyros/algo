use std::any::Any;
use std::fmt::Display;
use std::cmp::PartialEq;

pub fn eq<T: Any + Display + PartialEq>(given: T, expected: T) {
    assert!(given == expected, "{} != {}", given, expected);
}