

def get_top_level_tag_location(lines: list[str]) -> list[int]:
    tag_locations: list[int] = []
    for i in range(len(lines)):
        line = lines[i]
        if line.startswith((' ', '\t')):
            continue
        tag_locations.append(i)
    return tag_locations


def get_tag_location(lines: list[str], tag:str) -> list[int]:
    tag_locations: list[int] = []
    for i in range(len(lines)):
        line = lines[i]
        if not line.endswith(tag):
            continue
        tag_locations.append(i)
    return tag_locations


def get_tag_range(tag_locations: list[int]) -> list[tuple]:
    tag_ranges: list[tuple] = []
    if len(tag_locations) == 0:
        return tag_ranges    
    for j in range(int(len(tag_locations)/2)):
        index = j*2
        start = tag_locations[index]
        end = tag_locations[index+1] + 1
        tag_ranges.append((start, end))
    return tag_ranges