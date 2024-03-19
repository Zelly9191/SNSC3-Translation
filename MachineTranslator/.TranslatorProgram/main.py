import os
from JEObject import JEObject
from translator import Translator
import translation_formatter

cur_dir = os.getcwd()
machine_translated_dir = os.path.join(cur_dir, ".Z_Machine")

def make_directory():
    if not os.path.exists(machine_translated_dir):
        os.makedirs(machine_translated_dir)


def make_translated_xml(src_filename: str, dest_filename: str, t: Translator):
    xml = JEObject(src_filename)
    xml.get_data()
    xml.get_translation(t)
    xml.make_xml(dest_filename)


def run_program(t: Translator):
    # make needed directories
    make_directory()
    # loop through dir
    files = os.listdir(cur_dir)
    files_in_dir = len(files) - 4
    i = 0
    print(f"\n\n\t\tStarting auto-translation on {files_in_dir} files...\n\n")
    print("-----------------------------------------------------------------")
    for file in files:
        if not file.endswith(('.txt', '.xml')):
            continue
        print(f"\t\tTranslating {file}...\n\n")
        src_file = os.path.join(cur_dir, file)
        dest_file = machine_translated_dir + file
        dest_file = os.path.join(machine_translated_dir, file)
        if os.path.exists(dest_file):
            print(f"\t\t{file} already translated...\n\n")
            continue
        make_translated_xml(src_file, dest_file, t)
        i += 1
        print(f"\t\t{i}/{files_in_dir} files translated...\n\n")
        print("-----------------------------------------------------------------")


def test_file(filename, t):
    xml = JEObject(filename)
    xml.get_data()
    xml.get_translation(t)
    xml.make_xml("test_" + filename)


def main():
    translation_formatter.load_key_terms()
    t = Translator()
    run_program(t)
    # test_file('026_24933404_17c741c.xml', t)
    # test_file('011_24297628_172c09c.xml', t)
    # test_file('000_24682428_1789fbc.xml', t)


if __name__ == '__main__':
    main()
