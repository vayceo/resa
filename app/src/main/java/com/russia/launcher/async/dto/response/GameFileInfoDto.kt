package com.russia.launcher.async.dto.response

import java.io.Serializable

class GameFileInfoDto : Serializable {
    var allFilesSize: Long? = null
    var files: MutableList<FileInfo> = mutableListOf()
}