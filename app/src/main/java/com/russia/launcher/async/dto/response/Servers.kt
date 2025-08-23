package com.russia.launcher.async.dto.response

import com.google.gson.annotations.Expose
import com.google.gson.annotations.SerializedName
import java.io.Serializable

data class Servers (
    val color: String = "",
    val lock: Int = 0,
    val serverID: String = "",
    val mult: String = "",
    val name: String = "",
    val online: Int = 0,
    val maxonline: Int = 0,
    var port: Int = 0,
    var ip: String = "",
    var onlinePer30Min: Int = 123
)