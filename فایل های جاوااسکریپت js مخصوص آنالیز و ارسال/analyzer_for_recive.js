
function main(inputJsonString, outputJsonString) {
        
    var inputJson = inputJsonString;
    var outputJson = JSON.parse(outputJsonString);

    if(inputJson=="ch1on"){
        outputJson.status.push({
            key: "ch1",
            value: "on"
        });
        outputJson.result = "OK";

    }else if(inputJson=="ch1off"){

        outputJson.status.push({
            key: "ch1",
            value: "off"
        });
        outputJson.result = "OK";

    } else if(inputJson=="ch2on"){
        outputJson.status.push({
            key: "ch2",
            value: "on"
        });
        outputJson.result = "OK";

    }else if(inputJson=="ch2off"){
        outputJson.status.push({
            key: "ch2",
            value: "off"
        });
        outputJson.result = "OK";
    }else{
        outputJson.result = 'Invalid command!';
    }

    return JSON.stringify(outputJson);
}