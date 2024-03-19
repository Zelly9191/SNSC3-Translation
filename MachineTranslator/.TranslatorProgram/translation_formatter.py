import pandas as pd
import os

TERMS: pd.DataFrame
SJIS = 'SJIS'
TRANSLATOR = "TRANSLATOR"
ASCII = "ASCII"

TAGSDIR = os.path.join(os.getcwd(), ".TranslatorProgram", "TAGS.csv")
PEOPLEDIR = os.path.join(os.getcwd(), ".TranslatorProgram", "PEOPLE.csv")
PLACESDIR = os.path.join(os.getcwd(), ".TranslatorProgram", "PLACES.csv")
MISCDIR = os.path.join(os.getcwd(), ".TranslatorProgram", "MISC.csv")

def load_key_terms():
    tags = pd.read_csv(TAGSDIR, dtype=str)
    people = pd.read_csv(PEOPLEDIR, dtype=str)
    places = pd.read_csv(PLACESDIR, dtype=str)
    misc = pd.read_csv(MISCDIR, dtype=str)
    frames = [tags, people, places, misc]
    global TERMS
    TERMS = pd.concat(frames, ignore_index=True)
    TERMS.index = TERMS[SJIS].str.len()
    TERMS = TERMS.sort_index(ascending=False).reset_index(drop=True)


def format_translations(t_objs: list[dict]):
    new_t_objs: list[dict] = []
    for t_obj in t_objs:
        new_t_obj = format_translation(t_obj)
        new_t_objs.append(new_t_obj)
    return new_t_objs


def format_translation(t_obj: dict):
    new_t_obj: dict = {}
    japanese: str = t_obj['japanese']
    romaji: str = t_obj['romaji']
    english: list[str] = t_obj['ascii']
    new_romaji = format_ascii(romaji)
    new_english = []
    for line in english:
        new_english_line = format_ascii(line)
        new_english.append(new_english_line)
    new_t_obj['japanese'] = japanese
    new_t_obj['romaji'] = new_romaji
    new_t_obj['ascii'] = new_english
    return new_t_obj


def format_sjis(sjis: str):
    new_sjis = sjis
    new_sjis = new_sjis.replace('\t', '')
    # this is an special case.
    # I sort TERMS by string length so this space comes up last, but it needs to be before most things
    new_sjis = new_sjis.replace('\u3000', '[SPC]')
    new_sjis = new_sjis.replace('<end_line>', '')
    for i in range(len(TERMS)):
        sjis_word = TERMS[SJIS][i]
        if sjis_word not in new_sjis:
            continue
        translator_word = TERMS[TRANSLATOR][i]
        new_sjis = new_sjis.replace(sjis_word, translator_word)
    return new_sjis


def format_ascii(line: str):
    new_line = line.lower()
    for i in range(len(TERMS)):
        translator_word = TERMS[TRANSLATOR][i].lower()
        if translator_word not in new_line:
            continue
        ascii_word = TERMS[ASCII][i]
        if ascii_word == "\u3000":
            new_line = new_line.replace(" " + translator_word + " ", ascii_word)
        else:
            new_line = new_line.replace(translator_word, ascii_word)
    new_line = new_line + "<end_line>"
    return new_line
