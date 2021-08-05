Blockly.Blocks['aht20_read'] = {
	init: function() {
		this.jsonInit({
			"type": "aht20_read",
			"message0": Blockly.Msg.AHT20_READ_MESSAGE,
			"args0": [
				{
					"type": "field_dropdown",
					"name": "type",
					"options": [
					[
						"temperature (°C)",
						"0"
					],
					[
						"humidity (%RH)",
						"1"
					]
					]
				}
			],
			"output": "Number",
			"colour": 135,
			"tooltip": Blockly.Msg.AHT20_READ_TOOLTIP,
			"helpUrl": "https://www.inex.co.th/"
		})
	}
};
