import helper_functions as hp
from node import Node
from translator import Translator


class JEObject:
    def __init__(self, xml) -> None:
        self.xml = xml
        self.lines: list[str] = []
        self.new_lines: list[str] = []
        self.nodes: list[Node] = []
    
    def get_data(self):
        self._read_file()
        self._get_nodes()
        self._setup_nodes()

    def get_translation(self, t: Translator):
        self._translate_nodes(t)
        self._alter_nodes()

    def make_xml(self, filename):
        self._get_new_lines()
        self._to_xml(filename)

    def _read_file(self):
        with open(self.xml, 'r', encoding="utf-8") as f:
            lines = f.readlines()
        self.lines = [sub.replace("\n", '') for sub in lines]

    def _get_nodes(self):
        tag_locations = hp.get_top_level_tag_location(self.lines)
        tag_ranges = hp.get_tag_range(tag_locations)
        for k in tag_ranges:
            start = k[0]
            end = k[1]
            node = Node(self.lines[start:end])
            self.nodes.append(node)

    def _setup_nodes(self):
        for node in self.nodes:
            node.get_sjis()

    def _translate_nodes(self, t: Translator):
        for node in self.nodes:
            for line in node.sjis:
                try:
                    t_obj = t.get_translation(line)
                    node.add_t_object(t_obj)
                except Exception as e:
                    print(e)

    def _alter_nodes(self):
        for node in self.nodes:
            node.add_translation()

    def _get_new_lines(self):
        for node in self.nodes:
            self.new_lines = self.new_lines + node.lines

    def _to_xml(self, filename):
        file = filename
        lines = [sub + "\n" for sub in self.new_lines]
        with open(file, 'w', encoding="utf-8") as f:
            f.writelines(lines)
