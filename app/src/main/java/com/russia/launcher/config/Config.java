package com.russia.launcher.config;

import com.russia.launcher.NetworkService;

public class Config {
    public static final String APK_FILE_NAME = "russia.apk";

    public static final String ZIP_FILES_BASE_ADR = NetworkService.FILES_BASE_ADR + "zip/";
    public static final String FILE_INFO_URL = NetworkService.FILES_BASE_ADR + "files.php";

    public static final String URL_RE_CAPTCHA = "https://files.russia.online/reCaptcha.html";
    private static final String URL_DONATE = "https://russia.online/donate_v2/confirm.php?server=%s&serverName=%s&sum=%s&account=%s&mail=%s&captcha=%s";

    public static final String LIVE_RUSSIA_RESOURCE_SERVER_URL = "https://files.russia.online";
    public static final String FORUM_URL = "https://forum.russia.online";
    public static final String DONATE_URL = "https://russia.online/donate/";

    public static final String DISCORD_URI = "https://discord.com/invite/pkT6SEEXKS";
    public static final String VK_URI = "https://vk.com/russia.online";
    public static final String YOU_TUBE_URI = "https://www.youtube.com/@LeonidLitvinenko";
    public static final String TELEGRAM_URI = "https://t.me/lr_dev";

    public static final String NATIVE_SETTINGS_FILE_PATH = "/SAMP/settings.ini";
    public static final String SETTINGS_FILE_PATH = "/gta_sa.set";

    public static String createBillingUri(String serverId, String serverName, String sum, String nickname, String mail, String captcha) {
        return String.format(
                URL_DONATE,
                serverId,
                serverName,
                sum,
                nickname,
                mail,
                captcha
        );
    }
}