import esphome.codegen as cg

def load_ir_remote():
    # compat with recent Arduino framework
    # https://github.com/crankyoldgit/IRremoteESP8266/pull/2144
    cg.add_library("IRremoteESP8266", None, "https://github.com/BorisKofman/IRremoteESP8266#Espressif-version-3")
