import math

SAMPLE = ["1163751742",
          "1381373672",
          "2136511328",
          "3694931569",
          "7463417111",
          "1319128137",
          "1359912421",
          "3125421639",
          "1293138521",
          "2311944581"]

BIG_GRID = [
    "11637517422274862853338597396444961841755517295286",
    "13813736722492484783351359589446246169155735727126",
    "21365113283247622439435873354154698446526571955763",
    "36949315694715142671582625378269373648937148475914",
    "74634171118574528222968563933317967414442817852555",
    "13191281372421239248353234135946434524615754563572",
    "13599124212461123532357223464346833457545794456865",
    "31254216394236532741534764385264587549637569865174",
    "12931385212314249632342535174345364628545647573965",
    "23119445813422155692453326671356443778246755488935",
    "22748628533385973964449618417555172952866628316397",
    "24924847833513595894462461691557357271266846838237",
    "32476224394358733541546984465265719557637682166874",
    "47151426715826253782693736489371484759148259586125",
    "85745282229685639333179674144428178525553928963666",
    "24212392483532341359464345246157545635726865674683",
    "24611235323572234643468334575457944568656815567976",
    "42365327415347643852645875496375698651748671976285",
    "23142496323425351743453646285456475739656758684176",
    "34221556924533266713564437782467554889357866599146",
    "33859739644496184175551729528666283163977739427418",
    "35135958944624616915573572712668468382377957949348",
    "43587335415469844652657195576376821668748793277985",
    "58262537826937364893714847591482595861259361697236",
    "96856393331796741444281785255539289636664139174777",
    "35323413594643452461575456357268656746837976785794",
    "35722346434683345754579445686568155679767926678187",
    "53476438526458754963756986517486719762859782187396",
    "34253517434536462854564757396567586841767869795287",
    "45332667135644377824675548893578665991468977611257",
    "44961841755517295286662831639777394274188841538529",
    "46246169155735727126684683823779579493488168151459",
    "54698446526571955763768216687487932779859814388196",
    "69373648937148475914825958612593616972361472718347",
    "17967414442817852555392896366641391747775241285888",
    "46434524615754563572686567468379767857948187896815",
    "46833457545794456865681556797679266781878137789298",
    "64587549637569865174867197628597821873961893298417",
    "45364628545647573965675868417678697952878971816398",
    "56443778246755488935786659914689776112579188722368",
    "55172952866628316397773942741888415385299952649631",
    "57357271266846838237795794934881681514599279262561",
    "65719557637682166874879327798598143881961925499217",
    "71484759148259586125936169723614727183472583829458",
    "28178525553928963666413917477752412858886352396999",
    "57545635726865674683797678579481878968159298917926",
    "57944568656815567976792667818781377892989248891319",
    "75698651748671976285978218739618932984172914319528",
    "56475739656758684176786979528789718163989182927419",
    "67554889357866599146897761125791887223681299833479"]

with open('day15.input', 'r') as fin:
    DATA = [line.strip() for line in fin.readlines()]


def parse(data):
    grid = {}
    width = len(data)
    height = len(data[0])
    for y in range(height):
        for x in range(width):
            grid[(x, y)] = int(data[y][x])
    return grid, width, height


class Node():
    def __init__(self, parent=None, position=None) -> None:
        self.parent = parent
        self.position = position

        self.g = 0
        self.h = 0
        self.f = 0

    def __eq__(self, other) -> bool:
        return self.position == other.position


def calculate_h(from_tile, to_tile):
    return int(math.sqrt((to_tile[0] - from_tile[0]) ** 2 + (to_tile[1] - from_tile[1]) ** 2))


def is_valid(t, width, height):
    return t[0] >= 0 and t[1] >= 0 and t[0] < width and t[1] < height


def find_path(grid, width, height, from_tile, to_tile):
    start_node = Node(None, from_tile)
    end_node = Node(None, to_tile)

    open_list = []
    closed_list = set()

    open_list.append(start_node)

    while open_list:
        current_node = open_list[0]
        current_index = 0

        open_list.pop(current_index)
        closed_list.add(current_node.position)

        if current_node == end_node:
            path = []
            node = current_node
            while node:
                path.append(node.position)
                node = node.parent

            return path[::-1]

        neightbours = []
        for n in {(-1, 0), (0, -1), (1, 0), (0, 1)}:
            next_node_position = (
                current_node.position[0] + n[0], current_node.position[1] + n[1])

            if not is_valid(next_node_position, width, height):
                continue

            if next_node_position in closed_list:
                continue

            neightbours.append(Node(current_node, next_node_position))

        for n in neightbours:
            n.g = current_node.g + grid[n.position]
            n.h = calculate_h(n.position, end_node.position)
            n.f = n.g + n.h

            if len([open_node for open_node in open_list if open_node == n and open_node.g <= n.g]) > 0:
                continue

            open_list.append(n)
            open_list.sort(key=lambda x: x.f)

    return None


def part_1(grid, width, height):
    path = find_path(grid, width, height, (0, 0), (width - 1, height - 1))

    risk_level = 0
    for p in path[1:]:
        risk_level += grid[p]

    return risk_level


def multiple_grid(grid, width, height, times):
    big_grid = {}
    for k, v in grid.items():
        for y in range(times):
            for x in range(times):
                new_v = v + y + x
                if new_v > 9:
                    new_v -= 9
                big_grid[(k[0] + x * width, k[1] + y * height)] = new_v
    return big_grid, width * times, height * times


def part_2(grid, width, height):
    grid, width, height = multiple_grid(grid, width, height, 5)
    return part_1(grid, width, height)


grid, width, height = parse(DATA)
print(part_1(grid, width, height))
print(part_2(grid, width, height))
