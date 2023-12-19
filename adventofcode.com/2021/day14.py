from collections import defaultdict
from collections import Counter

SAMPLE = [
    "NNCB",
    "",
    "CH -> B",
    "HH -> N",
    "CB -> H",
    "NH -> C",
    "HB -> C",
    "HC -> B",
    "HN -> C",
    "NN -> C",
    "BH -> H",
    "NC -> B",
    "NB -> B",
    "BN -> B",
    "BB -> N",
    "BC -> B",
    "CC -> N",
    "CN -> C"]

with open('day14.input', 'r') as fin:
    DATA = [line.strip() for line in fin.readlines()]

def parse(data):
    rules = dict()
    for l in data[2:]:
        pair, p = l.split(' -> ')
        rules[pair] = p
    return data[0], rules


def apply(template, rules):
    result = template[0]
    for i in range(len(template) - 1):
        key = template[i:i+2]
        value = rules[key]
        result = result + value + key[1]
    return result


def part_1(template, rules, steps):
    for i in range(steps):
        template = apply(template, rules)
        
    count = defaultdict(lambda: 0)
    for c in template:
        count[c] = count[c] + 1
        
    lowest = min(count.values())
    highest = max(count.values())

    return highest - lowest


def part_2(template, rules, steps):
    counts = Counter()
    for i in range(len(template)-1):
        counts[template[i:i+2]] += 1
    
    for _ in range(steps):
        new_counts = Counter()
        char_counts = Counter()
        for k, v in counts.items():
            new_counts[f'{k[0]}{rules[k]}'] += v
            new_counts[f'{rules[k]}{k[1]}'] += v
            
            char_counts[k[0]] += v
            char_counts[rules[k]] += v
        counts = new_counts
        
        char_counts[template[-1]] += 1

    lowest = min(char_counts.values())
    highest = max(char_counts.values())
    return highest - lowest


template, rules = parse(DATA)
result = part_1(template, rules, 10)
print(result)

result = part_2(template, rules, 40)
print(result)
