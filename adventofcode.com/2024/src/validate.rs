use std::any::Any;
use std::fmt::Display;
use std::cmp::PartialEq;

pub fn eq<T: Any + Display + PartialEq>(given: T, expected: T, message: &str) {
    println!("{}: {} {}", message, given, expected);
    assert!(given == expected, "{}: {} != {}", message, given, expected);
}