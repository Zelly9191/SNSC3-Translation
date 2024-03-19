import helper_functions as hp
import translation_formatter


class Node:    
    def __init__(self, lines) -> None:
        self.lines: list[str] = lines
        self.tabs: list[int] = []
        self.spaces: list[int] = []
        self.sjis_locations: list[tuple[int,int]] = []
        self.sjis: list[str] = []
        self.offset: int = 0
        self.t_objs: list[dict] = []
    
    def get_sjis(self):
        self.sjis_locations = self.get_sjis_location()
        for i in self.sjis_locations:
            # get tab count for formatting
            line = self.lines[i[0]]
            tabs = line.count("\t")
            space = line.count(" ")
            self.tabs.append(tabs)
            self.spaces.append(space)
            # move one line after tag starts <sjis>
            start = i[0] + 1
            # move one line before tag ends </sjis>
            end = i[1] - 1
            lines = self.lines[start : end]
            lines = ''.join(lines)
            lines = translation_formatter.format_sjis(lines)
            self.sjis.append(lines)

    def get_sjis_location(self):
        tag_locations = hp.get_tag_location(self.lines, "sjis>")
        tag_ranges = hp.get_tag_range(tag_locations)
        return tag_ranges
    
    def add_translation(self):
        for i in range(len(self.t_objs)):
            start = self.sjis_locations[i][1]
            self.add_romaji(i, start)
            self.add_machine_ascii(i, start)

    def add_romaji(self, i: int, start: int):
        romaji = self.t_objs[i]['romaji']
        lines = self.make_insert_lines(i, "romaji", romaji)
        self.insert(start, lines)

    def add_machine_ascii(self, i: int, start: int):
        ascii_text = self.t_objs[i]['ascii']
        for j in range(len(ascii_text)):
            lines = self.make_insert_lines(i, "machine_ascii", ascii_text[j])
            self.insert(start, lines)

    def insert(self, start: int, lines: list[str]):
        for line in lines:
            self.lines.insert(start + self.offset, line)
            self.offset += 1

    def make_insert_lines(self, index: int, tag: str, line: str) -> list[str]:
        tabs = "\t" * self.tabs[index]
        tabs_plus = "\t" * (self.tabs[index] + 1)
        spaces = " " * self.spaces[index]
        spaces_plus = " " * (self.spaces[index])
        real_tag1 = '<' + tag + '>'
        real_tag2 = '</' + tag + '>'
        if line is None:
            line = ''
        lines = [
            f"{tabs}{spaces}{real_tag1}",
            tabs_plus + spaces_plus + line,
            f"{tabs}{spaces}{real_tag2}"
        ]
        return lines
    
    def add_t_object(self, t_obj):
        self.t_objs.append(t_obj)
        self.format_english_translation()

    def format_english_translation(self):
        self.t_objs = translation_formatter.format_translations(self.t_objs)