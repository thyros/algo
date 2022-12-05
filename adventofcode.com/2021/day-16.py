import math
from functools import reduce
from typing import NamedTuple
from typing import List

SAMPLE_1 = "D2FE28"


def to_binary(data):
    CONVERT_TABLE = {'0': "0000",
                     '1': "0001",
                     '2': "0010",
                     '3': "0011",
                     '4': "0100",
                     '5': "0101",
                     '6': "0110",
                     '7': "0111",
                     '8': "1000",
                     '9': "1001",
                     'A': "1010",
                     'B': "1011",
                     'C': "1100",
                     'D': "1101",
                     'E': "1110",
                     'F': "1111"}

    binary = [CONVERT_TABLE[c] for c in data]
    return "".join(binary)


with open('day-16.input', 'r') as fin:
    DATA = fin.read()


def to_dec(binary):
    return reduce(lambda x, y: x * 2 + int(y), binary, 0)


class Stream():
    def __init__(self, data):
        self.data = data
        self.position = 0

    def read(self, bits):
        substr = self.data[self.position:self.position + bits]
        self.position += bits
        return to_dec(substr)


class Packet(NamedTuple):
    version: int
    type: int
    n: int = -1
    packets: List['Packet'] = ()


def parse_packet(data: str, i: int):
    def _read(d: str, i: int, n: int):
        ret = to_dec(d[i:i+n])
        i += n
        return i, ret

    i, version = _read(data, i, 3)
    i, type = _read(data, i, 3)

    if type == 4:
        i, chunk = _read(data, i, 5)
        value = chunk & 0b1111

        while chunk & 0b10000:
            i, chunk = _read(data, i, 5)
            value <<= 4
            value += chunk & 0b1111

        return i, Packet(version=version, type=type, n=value)
    else:
        i, mode = _read(data, i, 1)
        if mode == 0:
            i, total_length = _read(data, i, 15)
            j = i
            i = i + total_length
            packets = []
            while j < i:
                j, packet = parse_packet(data, j)
                packets.append(packet)

            ret = Packet(version=version,
                         type=type,
                         packets=tuple(packets))

            return i, ret
        else:
            i, total_packages = _read(data, i, 11)
            packets = []
            for _ in range(total_packages):
                i, packet = parse_packet(data, i)
                packets.append(packet)

            ret = Packet(version=version,
                         type=type,
                         packets=tuple(packets))
            return i, ret


def part_1(data: str) -> int:
    _, packet = parse_packet(data, 0)
    total = 0
    packets_to_scan = [packet]
    while packets_to_scan:
        p = packets_to_scan.pop()
        total += p.version
        packets_to_scan.extend(p.packets)

    return total


def part_2(data: str) -> int:
    _, packet = parse_packet(data, 0)

    def value(packet: Packet) -> int:
        v = [value(p) for p in packet.packets]
        if packet.type == 0:
            return sum(v)
        if packet.type == 1:
            return math.prod(v)
        if packet.type == 2:
            return min(v)
        if packet.type == 3:
            return max(v)
        if packet.type == 4:
            return packet.n
        if packet.type == 5:
            return 1 if v[0] > v[1] else 0
        if packet.type == 6:
            return 1 if v[0] < v[1] else 0
        if packet.type == 7:
            return 1 if v[0] == v[1] else 0

    return value(packet)


data = to_binary(DATA)
value = part_2(data)
print(value)
