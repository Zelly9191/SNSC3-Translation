import time
import googletrans


class Translator():
    def __init__(self) -> None:
        self.t = googletrans.Translator()
        self.translation_limit = 100
        self.translations = 0
        self.timer = 60

    def get_translation(self, japon: str):
        tObj = {}
        if self.translations > self.translation_limit:
            self.translations = 0
            print("Taking a 60 second break to avoid request spamming google translate.")
            time.sleep(self.timer)
        res = self.t.translate(text=japon, dest=googletrans.LANGUAGES['en'], src=googletrans.LANGUAGES['ja'])
        self.translations += 1
        tObj['japanese'] = japon
        tObj['romaji'] = res.extra_data['origin_pronunciation']
        enTranslation = []
        for x in res.extra_data['parsed'][1][0][0][5][0][4]:
            enTranslation.append(x[0])
        tObj['ascii'] = enTranslation
        return tObj