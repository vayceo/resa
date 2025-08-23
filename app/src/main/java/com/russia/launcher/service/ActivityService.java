package com.russia.launcher.service;

import android.app.Activity;
import android.content.Context;

public interface ActivityService {

    void showInfoMessage(String message, Activity activity);

    boolean isGameFileInstall(Activity activity, String filePath);

    void showBigMessage(String message, Activity activity);
}
