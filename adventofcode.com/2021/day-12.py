from collections import defaultdict

SAMPLE = ["start-A",
          "start-b",
          "A-c",
          "A-b",
          "b-d",
          "A-end",
          "b-end"]

SAMPLE_2 = ["dc-end",
            "HN-start",
            "start-kj",
            "dc-start",
            "dc-HN",
            "LN-dc",
            "HN-end",
            "kj-sa",
            "kj-HN",
            "kj-dc"]

SAMPLE_3 = ["fs-end",
            "he-DX",
            "fs-he",
            "start-DX",
            "pj-DX",
            "end-zg",
            "zg-sl",
            "zg-pj",
            "pj-he",
            "RW-he",
            "fs-DX",
            "pj-RW",
            "zg-RW",
            "start-pj",
            "he-WI",
            "zg-he",
            "pj-fs",
            "start-RW"]

with open('day-12.input', 'r') as fin:
    DATA = [line.strip() for line in fin.readlines()]


def parse(data):
    graph = defaultdict(list)

    for line in data:
        f, t = line.split('-')
        graph[f].append(t)
        graph[t].append(f)

    return graph


def find_paths(graph, can_visit, path_so_far, current_node, nodes_to_avoid):
    paths = []
    path_so_far.append(current_node)

    if current_node == 'end':
        return [path_so_far]

    if current_node.islower():
        nodes_to_avoid.append(current_node)

    for target_node in graph[current_node]:
        if can_visit(target_node, nodes_to_avoid):
            paths += find_paths(graph,
                                can_visit,
                                path_so_far.copy(),
                                target_node,
                                nodes_to_avoid.copy())

    return paths

def part_1(graph):
    def visit_nodes_once(node, nodes):
        return node not in nodes

    paths = find_paths(graph, visit_nodes_once, [], 'start', [])
    return paths

def part_2(graph):
    def visit_one_node_twice(node, nodes):
        if node == 'start':
            return False
        
        if node not in nodes:
            return True
        
        return not any(nodes.count(n) > 1 for n in nodes)

    paths = find_paths(graph, visit_one_node_twice, [], 'start', [])
    return paths

graph = parse(DATA)
paths = part_1(graph)
print(len(paths))

paths = part_2(graph)
print(len(paths))