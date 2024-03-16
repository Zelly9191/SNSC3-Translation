# Requirements
- `Python 3.10` - `3.10` or newer is recommended. It may work with older versions like `3.4` but that hasn't been checked!
- `pip` - Python packages manager
- `lxml` - XML utility python library. Esential to parse JE scripts (.xml files) and installable from `pip`

# Setup
1. Install `Python 3.10` from https://www.python.org/downloads/.
2. get `pip` using these instructions: https://pip.pypa.io/en/stable/installation/
3. Do `pip install lxml` to get `lxml` library.
4. Done.  

# Usage
### basic usage:
`.\ProgressReporter.py -dir "<path_to_dir_with_xml_files>" -out "<file_to_export_the_report>.csv"`
### help:
`.\ProgressReporter.py -h` this will print out simple instructions and parameters list: 
```
usage: ProgressReporter.py [-h] -dir DIR [-out [OUT]] [-v] [-skip_gendered] [-skip_locations] [-skip_others]

Find places in .xml scripts (JE Scripts) that are missing English translations

options:
  -h, --help       show this help message and exit
  -dir DIR         Path to folder(s) containing XML files to validate (It will be recursively scanned).
  -out [OUT]       a .csv file where the report should be written to.
  -v               use if you need more debug info
  -skip_gendered   use if you want to skip reporting missing translations inside of <male>/<female> tags
  -skip_locations  use if you want to skip reporting missing translations inside <location> tags
  -skip_others     use if you want to skip all other tags with untranslated text
```

### More examples:
- Report all texts except locations: 
`.\ProgressReporter.py -dir "..\..\..\JE_Scripts\" -out ".\report_main_20240316.csv" -skip_locations`
- Report only locations:
`.\ProgressReporter.py -dir "..\..\..\JE_Scripts\" -out ".\report_locations_20240316.csv" -skip_gendered -skip_others`

# How it works
1. Script scans recursively the directory given in `-dir` in search for any .xml file.
2. Then it loads the file contents with `utf-8` encoding and does a little preprocessing to fix some custom \<tags\> that are not properly closed (for more details check file `JEScripts_Preprocessor.py`).
3. At this point the contents are being parsed using `lxml` into a proper hierarchy tree and then we search for all `<sjis></sjis>` tags which contain JP text.
4. Iterating over all JP texts and searching for valid `<ascii></ascii>` EN translations. If no valid candidates have been found, we create an error message with filename, line number and parent node type, for report purposes.
5. Once all JP text have been checked, we create a nice `.csv` file with all the data.

