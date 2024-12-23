import argparse
import logging
import os.path
import requests
import urllib.request
import shutil

logger = logging.getLogger(__name__)

def parse_args():
    parser = argparse.ArgumentParser(
                        prog='Start New Day',
                        description='Deals with the boilerplate when starting a new for Advent of Code')

    parser.add_argument('day')
    return parser.parse_args()

def day_exists(day):
    return False
    files = [f"src/day{day:02}_part01.rs",
             f"src/day{day:02}_part02.rs",
             f"src/day{day:02}.sample",
             f"src/day{day:02}.input"]

    return any([os.path.isfile(f) for f in files])

def update_main(day):
    with open('src/main.rs', 'w') as fout:
        fout.write('mod validate;\n')
        fout.write('\n')

        fout.write(f'mod day{day:02}_part01;\n')
        fout.write(f'mod day{day:02}_part02;\n')

        fout.write('\n')
        fout.write('fn main() {\n')
        fout.write(f'    day{day:02}_part01::main();\n')
        fout.write(f'    day{day:02}_part02::main();\n')
        fout.write('}\n')


def start_new_day(day):
    logger.info(f"Starting new day '{day}'")

    if day_exists(day):
        logger.error(f"Day '{day}' already exists")
    else:
        create_rust_file(day)
        create_data_file(day)
        update_main(day)
        download_input(2024, day)

def create_rust_file(day):
    in_file = "day_template.rs"

    with open(in_file, "r") as fin:
        content = fin.read()
    content = content.replace("{day}", f"{day:02}")

    for part in [1, 2]:
        out_file = f"src/day{day:02}_part{part:02}.rs"
        with open(out_file, "w") as fout:
            fout.write(content.replace("{part}", f"{part:02}"))

def create_data_file(day):
    open(f"src/day{day:02}.sample", "a").close()

def download_input(year, day):
    url = f"https://adventofcode.com/{year}/day/{day}/input"
    out_file = f"src/day{day:02}.input"

    with open('session', 'r') as fin:
        session_text = fin.read()

    session = requests.Session()
    response = session.get(url,
        cookies={'session': session_text})

    if response.status_code == 200:
        with open(out_file, 'w') as fout:
            fout.write(response.text)
    else:
        logger.error(f"Failed to download: {url}")
        logger.error(f"\t{response}")

if __name__ == "__main__":
    logging.basicConfig(format="%(message)s", level=logging.INFO)

    args = parse_args()
    start_new_day(int(args.day))