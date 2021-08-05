Blockly.JavaScript['aht20_read'] = function(block) {
	var dropdown_type = block.getFieldValue('type');
	
	var code = `DEV_I2C1.AHT20(0, 0x38).read(${dropdown_type})`;
	return [code, Blockly.JavaScript.ORDER_NONE];
};
