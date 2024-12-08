import argparse
import logging
import urllib.request
import shutil

logger = logging.getLogger(__name__)

def parse_args():
    parser = argparse.ArgumentParser(
                        prog='Start New Day',
                        description='Deals with the boilerplate when starting a new for Advent of Code')

    parser.add_argument('day')
    return parser.parse_args()


def start_new_day(day):
    logger.info(f"Starting new day '{day}'")
    create_rust_file(day)
    create_data_file(day)
    # download_input(2024, day)

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
    open(f"src/day{day:02}.input", "a").close()

def download_input(year, day):
    url = f"https://adventofcode.com/{year}/day/{day}/input"
    output = f"{day:02}.input"

    logger.error(f"Downloading: {url}")
    logger.error(f"  into: {output}")
    urllib.request.urlretrieve(url, output)


if __name__ == "__main__":
    logging.basicConfig(format="%(message)s", level=logging.INFO)

    args = parse_args()
    start_new_day(int(args.day))