
function main(inputJsonString) {
    // Parse the input JSON string into a JavaScript object
    var inputJson = JSON.parse(inputJsonString);

    var outPut = "";
    switch (inputJson.operationName) {
        case "ch1":
            if (inputJson.value == "on") {
                outPut = 'ch1on';
            } else if (inputJson.value == "off") {
                outPut = 'ch1off';
            }
            break;
        case "ch2":
            if (inputJson.value == "on") {
                outPut = 'ch2on';
            } else if (inputJson.value == "off") {
                outPut = 'ch2off';
            }
            break;
        default:
            outPut = "";
            break;
    }

    // Return the result as an object using ES5 syntax
    var result = { type: 'command', value: outPut };
    return JSON.stringify(result);
}