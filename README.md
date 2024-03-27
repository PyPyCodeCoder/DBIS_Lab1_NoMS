### A script to demonstrate how the program works and understand the command syntax:

#### 1. Input 5 master records

```
insert-m 1 Universal_Pictures
```
```
insert-m 2 Paramount_Pictures
```
```
insert-m 3 Warner_Bros.
```
```
insert-m 4 Walt_Disney_Studios
```
```
insert-m 5 Sony_Picturs
```

#### 2. For 3 master records, enter 1, 2, and 3 slave records

```
insert-s 1 1 Kung_Fu_Panda_4 85000000
```
```
insert-s 2 1 Oppenheimer 100000000
```
```
insert-s 3 1 Despicable_Me_4 70000000
```
```
insert-s 4 3 Inception 1600000000
```
```
insert-s 5 3 Barbie 145000000
```
```
insert-s 6 5 Spider-Man:_Across_the_Spider-Verse 125000000
```

#### 3. ut-m, ut-s

```
ut-m
```
```
ut-s
```

#### 4. Delete a master record with two slaves

```
del-m 3
```

#### 5. Delete a slave record from a master record with 3 slaves

```
del-s 3 1
```

#### 6. ut-m, ut-s

```
ut-m
```
```
ut-s
```

#### 7. Enter another master record and a slave record to it

```
insert-m 6 20th_Century_Studios
```
```
insert-s 7 6 Ford_v_Ferrari 97000000
```

#### 8. ut-m, ut-s

```
ut-m
```
```
ut-s
```

#### 9. Update the master record field and the slave record field

```
update-m 5 Sony_Pictures
```
```
update-s 2 1 _ 160000000
```

#### 10. ut-m, ut-s

```
ut-m
```
```
ut-s
```
