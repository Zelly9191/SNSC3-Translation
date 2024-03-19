# Machine Translator

## requires googletrans to work
## python -m pip install googletrans

# INTERNET CONNECTION IS REQUIRED
Program connects to google translate, so no internet = no translation.

## Installation
Unzip in the directory with the xml.
A subdirectory will be made that will hold the translated files.
Run .translate.bat by double clicking it. I think it requires windows.
If you want to run the python directly,
put all the files in the directory where the XMLs are and run
python -m main
should still work

## Note about translations
Original files are not modified at all, only copied.
It takes a really long time. Depending on the amount of files being translated. Could take hours.
Typically anywhere from 2 seconds to 2 minutes per file for me, depending on the size.
Time will vary depending on better/worse computer.

If the file you run this on has already been translated, the translation will not dissappear.
Some sections will be put in-between the <sjis> and <ascii> sections,
but nothing from the original file should be deleted in any way.


# Possible Problems:
## Google Translate Timeout
Google Translate has some limitations on how much it can be used.
If translate requests happen too often it will block them and the program will probably crash.
I tried putting in a workaround, but I am not fully sure if it works, because running the program until it crashes takes a long time.

## Crashing
If the program does crash, all generated files should be fine. Not corrupted or any missing translations or anything.
Just restart the program (maybe wait a bit if it crashes soon after restarting it.)
Program can detect if a file has been translated already and will skip over it,
so it can pick up where it left off.
If you want to regenerate a machine translated file, delete it from the .Z_Machine folder and run program again

## Errors
You will probably see a "'NoneType' object has no attribute 'lower'" message in the console a lot.
This typically happens with really short lines that consist of only <tags> or only,
key names, places, and terms defined in the CSV files.

When looking at the files with the above described error, the <romaji> or <machine_ascii> text may look weird.
It may have some text inside square brackets [LiKeThiS] with weird capitalization.
I use these to determine words that will be manually translated, like tags and names etc.,
and the code wasn't able to pick up on it for some reason.
The text in brackets [] should be found in one of the CSVs if you need to know what it is supposed to be.


# IMPORTANT NOTE:

## New Tags
In the translated file, new sections are created under the japanese text.
There are <romaji> tags which display the romaji translation of the <sjis> text,
and the <machine_ascii> tags which display the google translated english text.
Translators should make their own english translation in an <ascii></ascii> section.

## Accuracy
All the japanese text is translated by a machine so the accuracy is of course not great.
It translates into romaji (japanese in english alphabet) and english.
I figured romaji would help some people, myself included.

From what I've seen so far, the accuracy of the romaji is pretty high.
I've only seen a few cases where it is translated weird.
For example it translates: blacksmith 鍛冶屋(kajiya) to tan'ya, which isn't japanese I think.
It can still translate tan'ya to blacksmith which is weird though.

There are many cases where the english translations are not good, but many cases where they are pretty okay.
For example if one japanese text seems to contain two sentences, it only translates the first.
Sometimes it completely skips over words. Who knows why.

### Haha
I just made this because I figured it could help out a good bit.
