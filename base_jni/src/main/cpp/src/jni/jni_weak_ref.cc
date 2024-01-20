#include "jni/jni_weak_ref.h"

#include "jni/jni_util.h"
#include "log/logging.h"

namespace FOREVER {
namespace JNI {

JavaObjectWeakGlobalRef::JavaObjectWeakGlobalRef() : obj_(NULL) {}

JavaObjectWeakGlobalRef::JavaObjectWeakGlobalRef(
    const JavaObjectWeakGlobalRef& orig)
    : obj_(NULL) {
  Assign(orig);
}

JavaObjectWeakGlobalRef::JavaObjectWeakGlobalRef(JNIEnv* env, jobject obj)
    : obj_(env->NewWeakGlobalRef(obj)) {
  FOREVER_DCHECK(obj_);
}

JavaObjectWeakGlobalRef::~JavaObjectWeakGlobalRef() { reset(); }

void JavaObjectWeakGlobalRef::operator=(const JavaObjectWeakGlobalRef& rhs) {
  Assign(rhs);
}

void JavaObjectWeakGlobalRef::reset() {
  if (obj_) {
    AttachCurrentThread()->DeleteWeakGlobalRef(obj_);
    obj_ = NULL;
  }
}

ScopedJavaLocalRef<jobject> JavaObjectWeakGlobalRef::get(JNIEnv* env) const {
  return GetRealObject(env, obj_);
}

ScopedJavaLocalRef<jobject> GetRealObject(JNIEnv* env, jweak obj) {
  jobject real = NULL;
  if (obj) {
    real = env->NewLocalRef(obj);
    if (!real) {
      FOREVER_DLOG(ERROR) << "The real object has been deleted!";
    }
  }
  return ScopedJavaLocalRef<jobject>(env, real);
}

void JavaObjectWeakGlobalRef::Assign(const JavaObjectWeakGlobalRef& other) {
  if (&other == this) {
    return;
  }

  JNIEnv* env = AttachCurrentThread();
  if (obj_) {
    env->DeleteWeakGlobalRef(obj_);
  }

  obj_ = other.obj_ ? env->NewWeakGlobalRef(other.obj_) : NULL;
}

}  // namespace JNI
}  // namespace FOREVER
