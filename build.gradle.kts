// Top-level build file where you can add configuration options common to all sub-projects/modules.
plugins {
    id("com.android.application") version "8.2.1" apply false
    id("org.jetbrains.kotlin.android") version "1.9.22" apply false
    id("com.android.library") version "8.2.1" apply false
    // 发布aar包
    // https://github.com/facebook/react-native/blob/92b889b89e92bb52ba2d6868bf6de4d7c3972368/packages/react-native/gradle/libs.versions.toml#L78
    id("io.github.gradle-nexus.publish-plugin") version "1.3.0"
    // gradle下载文件
    id("de.undercouch.download") version "5.4.0" apply false
    id("org.jetbrains.kotlinx.binary-compatibility-validator") version "0.13.2" apply true
}

val sonatypeUsername = findProperty("SONATYPE_USERNAME")?.toString()
val sonatypePassword = findProperty("SONATYPE_PASSWORD")?.toString()