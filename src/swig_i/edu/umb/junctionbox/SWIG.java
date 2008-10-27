package edu.umb.junctionbox;
public abstract class SWIG {
  protected abstract long getPointer();

  public boolean equals(Object obj) {
    boolean equal = false;
    if (obj instanceof SWIG)
      equal = (((SWIG)obj).getPointer() == this.getPointer());
    return equal;
  }

  SWIGTYPE_p_void getVoidPointer() {
    return new SWIGTYPE_p_void(getPointer(), false);
  }
}