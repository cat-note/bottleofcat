(arr.sort((x,y)=>x-y).reduce((p,n,i)=>{ // arr是包括所有进程运行时间的数组，例如[10,6,2,4,8]
    let sum=arr.slice(0,i).reduce((x,y)=>x+y);
    return p+sum+n;
})) / arr.length