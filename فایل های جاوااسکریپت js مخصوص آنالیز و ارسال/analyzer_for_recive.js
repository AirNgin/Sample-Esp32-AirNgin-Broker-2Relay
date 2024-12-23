function main(inputJsonString, outputJsonString) {

    // inputJsonString : دقیقا همان مقداری می باشد که شما در زمان ارسال وضعیت خود به سمت بروکر ارسال می کنید.
    // در کدهای همین پروژه می توانید فانکشن زیر را به عنوان مثال در نظر بگیرید.
    // Mqtt__Send("DeviceToServer", "cloud", "ch1on" , "", true);
    // در خط کد بالا : inputJsonString = ch1on 
    // ***** نکته  *******
    // شما می توانید داده های خود را بصورت json
    // نیر ازسال نمایید و یا حتی بجاز نوشته از اعداد استفاده نمایید مانند : 11،10،21،20
    var inputJson = inputJsonString;

    // این یک تابع json
    // ثابت از سمت سرور می باشد که نیاز است مطابق با که باید مقادیر آن را پر نماییم
    var outputJson = JSON.parse(outputJsonString);

    // شما باید پروتکل خود را با قوانین سرور منطبق نمایید
    // ما فرض می نماییم پروتکل ch1on
    // شما دقیقا همان ch1
    // در بروکر می باشد که حالت روشن یا همان on است
    // یک لیست می باشد که شامل دو مقدار زیر است
    // key , value
    // key : همان operationName می باشد
    // value : همان مقدار command می باشد
    // برای دریافت operationName و command های سیستمی به پنل و ابزار مورد نظر خود مراجعه نمایید.

    if(inputJson=="ch1on"){
    
    // برای اضافه کردن مقدار به outputJson از روش زیر استفاده می نماییم.
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
    // اگر هیچکدام از مقادیر بالا نبود خطا ثبت می شود
        outputJson.result = 'Invalid command!';
    }

    // بازگشت داده برای سرور جهت ارسال به نرم افزارهای موبایل و یا اجرای سناریو ها
    return JSON.stringify(outputJson);
}