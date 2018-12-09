#ifdef _PLATFORM_ANDROID

#include <string.h>
#include <jni.h>

#include "Platform.h"
#include "Platform_Android.h"

JNIEnv* _env;

extern "C"
{
	JNIEXPORT void Java_com_view_MyView_jniOnCreate( JNIEnv* env, jobject jObj );
	JNIEXPORT void Java_com_view_MyView_jniOnSurfaceCreated( JNIEnv* env, jobject jObj );
	JNIEXPORT void Java_com_view_MyView_jniOnSurfaceChanged( JNIEnv* env, jobject jObj, jint width, jint height );
    JNIEXPORT int Java_com_view_MyView_jniOnDrawFrame( JNIEnv* env, jobject jObj, jint deltaTimeInMillis );
    JNIEXPORT void Java_com_view_MyView_jniOnTouchEvent( JNIEnv* env, jobject jObj, jint x, jint y, jint eventID );
    JNIEXPORT void Java_com_view_MyView_jniOnPause( JNIEnv* env, jobject jObj );
    JNIEXPORT void Java_com_view_MyView_jniOnResume( JNIEnv* env, jobject jObj );
    JNIEXPORT void Java_com_view_MyView_jniOnStop( JNIEnv* env, jobject jObj );
    JNIEXPORT void Java_com_view_MyView_jniOnDestroy( JNIEnv* env, jobject jObj );
    JNIEXPORT void Java_com_view_MyView_jniSetAssetManager(JNIEnv* env, jobject jobj, jobject assetManager);
    JNIEXPORT void Java_com_view_MyView_jniSetSaveFolderPath(JNIEnv* env, jobject jobj, jstring saveFolderPath);
    JNIEXPORT void Java_com_view_MyView_jniSetLanguageID(JNIEnv* env, jobject jobj, jint languageID);
    JNIEXPORT void Java_com_view_MyView_jniSetBackKeyPressed(JNIEnv* env, jobject jobj);
    JNIEXPORT void Java_com_view_MyView_jniSetAccMeterMoved(JNIEnv* env, jobject jobj, jint dir);
};

JNIEXPORT void Java_com_view_MyView_jniOnCreate( JNIEnv* env, jobject jObj )
{
	Platform::GetPlatform()->onCreate();
}

JNIEXPORT void Java_com_view_MyView_jniOnSurfaceCreated( JNIEnv* env, jobject jObj )
{
	//Platform::GetPlatform()->onCreate();
}

JNIEXPORT void Java_com_view_MyView_jniSetAssetManager(JNIEnv* env, jobject jobj, jobject assetManager)
{
	Platform_Android* platformAndroid = (Platform_Android*)Platform::GetPlatform();
	platformAndroid->SetAssetManager( AAssetManager_fromJava(env, assetManager) );
}

JNIEXPORT void Java_com_view_MyView_jniOnSurfaceChanged( JNIEnv* env, jobject jObj, jint width, jint height )
{

	Platform::GetPlatform()->onSizeChange(width, height);
}

JNIEXPORT int Java_com_view_MyView_jniOnDrawFrame( JNIEnv* env, jobject jObj, jint deltaTimeInMillis )
{
	_env = env;
	return Platform::GetPlatform()->Draw(deltaTimeInMillis);
}

JNIEXPORT void Java_com_view_MyView_jniOnTouchEvent( JNIEnv* env, jobject jObj, jint x, jint y, jint eventID )
{
	Platform::GetPlatform()->onTouch(x, y, eventID);
}

JNIEXPORT void Java_com_view_MyView_jniOnPause( JNIEnv* env, jobject jObj )
{
	Platform::GetPlatform()->onPause();
}

JNIEXPORT void Java_com_view_MyView_jniOnResume( JNIEnv* env, jobject jObj )
{
	Platform::GetPlatform()->onResume();
}

JNIEXPORT void Java_com_view_MyView_jniOnStop( JNIEnv* env, jobject jObj )
{
	Platform::GetPlatform()->onPause();
}

JNIEXPORT void Java_com_view_MyView_jniOnDestroy( JNIEnv* env, jobject jObj )
{
	Platform::GetPlatform()->onExit();
}

JNIEXPORT void Java_com_view_MyView_jniSetSaveFolderPath(JNIEnv* env, jobject jobj, jstring saveFolderPath)
{
	const char *path = env->GetStringUTFChars(saveFolderPath, NULL);
	Platform::GetPlatform()->SetSaveFolderPath(path);
	env->ReleaseStringUTFChars(saveFolderPath, path);
}

JNIEXPORT void Java_com_view_MyView_jniSetLanguageID(JNIEnv* env, jobject jobj, jint languageID)
{
	Platform_Android* platformAndroid = (Platform_Android*)Platform::GetPlatform();
	platformAndroid->SetLanguageID(languageID);
}

JNIEXPORT void Java_com_view_MyView_jniSetBackKeyPressed(JNIEnv* env, jobject jobj)
{
	Platform::GetPlatform()->SetBackKeyPressed();
}

JNIEXPORT void Java_com_view_MyView_jniSetAccMeterMoved(JNIEnv* env, jobject jobj, jint dir)
{
	Platform_Android* platformAndroid = (Platform_Android*)Platform::GetPlatform();
	platformAndroid->SetAccMeterMoved(dir);
}


//Callbacks to JAVA

void PlayMusic_native(bool variable)
{
	jclass classID = (_env)->FindClass("com/view/MyView");
	jmethodID methodID = (_env)->GetStaticMethodID(classID, "PlayMusic", "(Z)V");
	(_env)->CallStaticVoidMethod(classID, methodID, variable);
}

void PlaySound_native(int soundId, bool play, bool loop)
{
	jclass cls = (_env)->FindClass("com/view/MyView");
	jmethodID mid = (_env)->GetStaticMethodID(cls, "PlaySound", "(IZZ)V");
	return (_env)->CallStaticVoidMethod(cls, mid, soundId, play, loop);
}

bool IsMusicPlaying_native()
{
	jclass cls = (_env)->FindClass("com/view/MyView");
	jmethodID mid = (_env)->GetStaticMethodID(cls, "IsMusicPlaying", "()Z");
	return (_env)->CallStaticBooleanMethod(cls, mid);
}

void OnAudioPause_native()
{
	jclass classID = (_env)->FindClass("com/view/MyView");
	jmethodID methodID = (_env)->GetStaticMethodID(classID, "OnAudioPause", "()V");
	(_env)->CallStaticVoidMethod(classID, methodID);
}

void OnAudioResume_native()
{
	jclass classID = (_env)->FindClass("com/view/MyView");
	jmethodID methodID = (_env)->GetStaticMethodID(classID, "OnAudioResume", "()V");
	(_env)->CallStaticVoidMethod(classID, methodID);
}

float GetTimeInSeconds_native()
{
	jclass cls = (_env)->FindClass("com/view/MyView");
	jmethodID mid = (_env)->GetStaticMethodID(cls, "GetTimeInSeconds", "()F");
	return (_env)->CallStaticFloatMethod(cls, mid);
}

int GetTimeInMillis_native()
{
	jclass cls = (_env)->FindClass("com/view/MyView");
	jmethodID mid = (_env)->GetStaticMethodID(cls, "GetTimeInMillis", "()I");
	return (_env)->CallStaticIntMethod(cls, mid);
}

void ShowMsg_native(char* arr)
{
	jstring jStr = (_env)->NewStringUTF(arr);
	jclass cls = (_env)->FindClass("com/view/MyView");
	jmethodID mid = (_env)->GetStaticMethodID(cls, "ShowMsg", "(Ljava/lang/String;)V");
	(_env)->CallStaticVoidMethod(cls, mid, jStr);
}

void SleepThread_native(int timeInMillis)
{
	jclass cls = (_env)->FindClass("com/view/MyView");
	jmethodID mid = (_env)->GetStaticMethodID(cls, "SleepThread", "(I)V");
	(_env)->CallStaticVoidMethod(cls, mid, timeInMillis);
}

bool IsNetworkAvailable_native()
{
	jclass cls = (_env)->FindClass("com/view/MyView");
	jmethodID mid = (_env)->GetStaticMethodID(cls, "IsNetworkAvailable", "()Z");
	return (_env)->CallStaticBooleanMethod(cls, mid);
}

#endif
