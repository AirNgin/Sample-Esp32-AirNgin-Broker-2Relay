
function main(inputJsonString) {
    // Parse the input JSON string into a JavaScript object
    // inputJsonString: دقیقا دستوری می باشد که کاربر از نرم افزار موبایل در خواست داده است و می خواهد به سمت ابزار ارسال کند
    // تمامی این مقدار ها در زمان ثبت ابزار در قسمت دستورات به عنوان مقدار سیستمی تعریف شده است
    // تنها مقداری که شما تعریف می نمایید operationName
    // می باشد زیرا ما فضا را مهیا کرده ایم تا شما بتوانید از یک ابزار چند نمونه در یک محصول داشته باشد.
    // به طور مثال یک رله برد می تواند شامل 18 عدد رله باشد پس شما باید 18 عدد operationName
    // تعریف نمایید.
    var inputJson = JSON.parse(inputJsonString);

    var outPut = "";
    switch (inputJson.operationName) {
        case "ch1": //operationName تعریفی شما در پنل کاربری 
            if (inputJson.value == "on") { // مقدار سیستمی هر operationName
                outPut = 'ch1on'; // دستوری که ابزار  شما متوجه عملیات مورد انتظار می شود
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

    // Return the result
    // دقت نمایید شما می توانید خروجی خود را نیز بصورت json
    // به سرور ارایه دهید
    // توجه : سرور دقیقا مقدار outPut را برای ابزار شما ارسال می نماید
    // برای درک اینکه چطور باید این داده را آنالیز کنید به فایل iot.h
    // بخش دریافت داده از mqtt رجوع نمایید
    var result = { type: 'command', value: outPut };
    return JSON.stringify(result);
}