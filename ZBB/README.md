# ZBB
Measurement of the Z(->bb)+2jets production. Plotting weighted mass distribution

# Running fit on datacards 

To run maximum likelihood fit execute script RunFit_Data.bash. It takes as an argument the RooT filename with the datacard workspace w/o extension .root

```bash
./RunFit_Data.bash datacards_combined_ZBB_24_05_2021
```
This will create file mlfit_obs.root. 

# Running plotting macro on mlfit_obs.root. 

To produce S/(S+B) weighted mass plot execute RooT macro Plot.C

```bash
root -l Plot.C
```

This will create png file named Zbb.png
