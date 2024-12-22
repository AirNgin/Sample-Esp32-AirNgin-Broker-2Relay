Import("env")

# تعریف تابع پاک کردن حافظه فلش
def erase_flash(source, target, env):
    env.Execute("pio run -t erase")

# قبل از آپلود، حافظه فلش پاک شود
env.AddPreAction("upload", erase_flash)
