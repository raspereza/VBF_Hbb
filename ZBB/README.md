# ZBB
Measurement of the Z(->bb)+2jets production. Plotting weighted mass distribution

# Running fit on datacards with script RunFit_Data.bash. Scripts take as an argument the RooT filename with the datacard workspace w/o extension .root

```bash
./RunFit_Data.bash datacards_combined_ZBB_24_05_2021
```
This will create file mlfit_obs.root. 

`<year>/<channel>/htt_<category>.inputs-mssm-vs-sm-Run<year>-<variable>.root`

# Running plotting macro on mlfit_obs.root. 

To produce S/(S+B) weighted mass plot execute RooT macro Plot.C

```bash
root Plot.C
```

This will create png file named Zbb.png
