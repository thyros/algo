use std::ops::BitXor;

use itertools::Itertools;

use super::validate;

struct Machine {
    reg_a: i32,
    reg_b: i32,
    reg_c: i32,
    ip: usize,
    output: Vec<i32>,
}

fn combo(machine: &Machine, operand: i32) -> i32 {
    match operand {
        n if (0..=3).contains(&n) => n,
        4 => machine.reg_a,
        5 => machine.reg_b,
        6 => machine.reg_c,
        _ => {
            panic!("Invalid combo operand {}", operand);
        }
    }
}

// The adv instruction (opcode 0) performs division. The numerator is the value in the A register.
// The denominator is found by raising 2 to the power of the instruction's combo operand.
// (So, an operand of 2 would divide A by 4 (2^2); an operand of 5 would divide A by 2^B.) The result of the division operation is truncated to an integer and then written to the A register.
fn adv(machine: &mut Machine, operand: i32) {
    let operand_value = combo(machine, operand);
    let result = machine.reg_a / (2i32.pow(operand_value as u32));
    machine.reg_a = result;
    machine.ip += 2;
}

// The bxl instruction (opcode 1) calculates the bitwise XOR of register B and the instruction's literal operand, then stores the result in register B.
fn bxl(machine: &mut Machine, operand: i32) {
    let result = machine.reg_b.bitxor(operand);
    machine.reg_b = result;
    machine.ip += 2;
}

// The bst instruction (opcode 2) calculates the value of its combo operand modulo 8 (thereby keeping only its lowest 3 bits), then writes that value to the B register.
fn bst(machine: &mut Machine, operand: i32) {
    let operand_value = combo(machine, operand) % 8;
    machine.reg_b = operand_value;
    machine.ip += 2;
}

// The jnz instruction (opcode 3) does nothing if the A register is 0. However, if the A register is not zero,
// it jumps by setting the instruction pointer to the value of its literal operand; if this instruction jumps, the instruction pointer is not increased by 2 after this instruction.
fn jnz(machine: &mut Machine, operand: i32) {
    if machine.reg_a != 0 {
        machine.ip = operand as usize;
    } else {
        machine.ip += 2;
    }
}

// The bxc instruction (opcode 4) calculates the bitwise XOR of register B and register C, then stores the result in register B. (For legacy reasons, this instruction reads an operand but ignores it.)
fn bxc(machine: &mut Machine, _operand: i32) {
    let result = machine.reg_b.bitxor(machine.reg_c);
    machine.reg_b = result;
    machine.ip += 2;
}

// The out instruction (opcode 5) calculates the value of its combo operand modulo 8, then outputs that value. (If a program outputs multiple values, they are separated by commas.)
fn out(machine: &mut Machine, operand: i32) {
    let result = combo(machine, operand) % 8;
    machine.output.push(result);
    machine.ip += 2;
}

// The bdv instruction (opcode 6) works exactly like the adv instruction except that the result is stored in the B register. (The numerator is still read from the A register.)
fn bdv(machine: &mut Machine, operand: i32) {
    let operand_value = combo(machine, operand);
    let result = machine.reg_a / (2i32.pow(operand_value as u32));
    machine.reg_b = result;
    machine.ip += 2;
}

// The cdv instruction (opcode 7) works exactly like the adv instruction except that the result is stored in the C register. (The numerator is still read from the A register.)
fn cdv(machine: &mut Machine, operand: i32) {
    let operand_value = combo(machine, operand);
    let result = machine.reg_a / (2i32.pow(operand_value as u32));
    machine.reg_c = result;
    machine.ip += 2;
}

fn run_instruction(machine: &mut Machine, opcode: i32, operand: i32) {
    // println!("ip:{} a:{} b:{} c:{} opcode:{} operand:{}", machine.ip, machine.reg_a, machine.reg_b, machine.reg_c, opcode, operand);
    match opcode {
        0 => {
            adv(machine, operand);
        }
        1 => {
            bxl(machine, operand);
        }
        2 => {
            bst(machine, operand);
        }
        3 => {
            jnz(machine, operand);
        }
        4 => {
            bxc(machine, operand);
        }
        5 => {
            out(machine, operand);
        }
        6 => {
            bdv(machine, operand);
        }
        7 => {
            cdv(machine, operand);
        }
        _ => {
            panic!("Unsupported opcode {}", opcode)
        }
    }
}

fn solve(machine: &mut Machine, program: Vec<i32>) -> String {
    while machine.ip < program.len() {
        let opcode = program[machine.ip];
        let operand = program[machine.ip + 1];
        run_instruction(machine, opcode, operand);
    }

    // println!(
    //     "a:{} b:{} c:{} ",
    //     machine.reg_a, machine.reg_b, machine.reg_c
    // );

    let result = machine.output.iter().map(|o| o.to_string()).join(",");
    return result;
}

fn solve_sample() -> String {
    let mut machine = Machine {
        reg_a: 729,
        reg_b: 0,
        reg_c: 0,
        ip: 0,
        output: Vec::default(),
    };
    let program = vec![0, 1, 5, 4, 3, 0];

    return solve(&mut machine, program);
}

fn solve_input() -> String {
    let mut machine = Machine {
        reg_a: 46187030,
        reg_b: 0,
        reg_c: 0,
        ip: 0,
        output: Vec::default(),
    };
    let program = vec![2, 4, 1, 5, 7, 5, 0, 3, 4, 0, 1, 6, 5, 5, 3, 0];

    return solve(&mut machine, program);
}

pub fn main() {
    // let sample = include_str!("day17.sample").trim();
    validate::eq("4,6,3,5,6,3,5,2,1,0".to_string(), solve_sample(), "day17 part01 sample");

    // let input = include_str!("day17.input").trim();
    validate::eq("7,1,3,4,1,2,6,7,1".to_string(), solve_input(), "day17 part01 input");
}
