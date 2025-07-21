import esphome.codegen as cg
from esphome.components import climate_ir
from esphome.components import ir_remote_base

AUTO_LOAD = ["climate_ir", "ir_remote_base"]

electra_ns = cg.esphome_ns.namespace("electra")
ElectraClimate = electra_ns.class_("ElectraClimate", climate_ir.ClimateIR)

CONFIG_SCHEMA = climate_ir.climate_ir_with_receiver_schema(ElectraClimate)

async def to_code(config):
    ir_remote_base.load_ir_remote()

    await climate_ir.new_climate_ir(config)
