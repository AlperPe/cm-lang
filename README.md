**CM Language KURUMLARDAN bağımsız olarak Alper Pektaş tarafından kurgulanmış ve yazılmıştır.**
**CM Language was conceived and written by Alper Pektaş independently of any institutions.**
1. Library Imports & Variable DeclarationMultiple variables can be assigned in a single line without type keywords. 
If target is left empty (id("")) or undefined, the runtime automatically creates and targets a fallback directory.

Plaintextcall <library>
x = 10, y = 20, target = id("")

2. Conditionals & Chained Logic (->, and, or)Blocks open with -> instead of curly braces. 
A plain end closes only the current block, whereas end. with a dot terminates the entire program.
Plaintextif x == 1 ->
    save(target)
end id("main")

// Chained conditions and block routing
if x == 4 -> and y > 50 ->
    return block("main")
    print (y * id(""))
    save(target) id("k")
end id(random)

// Zero / null check to terminate a target block
if id("m") or id("a") == 0 ->
end id("z")

end. id(random)

3. Mathematical Intervals & Enforcement (make.)Chained inequalities 
(10 > x >= 5) are written directly in standard mathematical notation. 
The make. operator enforces equality when a condition holds and chains actions with and.
Plaintextif 10 > x >= 5 ->
    loop(id("m"), id("a"))
end

if x >= 10 ->
    make.(x == y) and print(f^(x * y))
end. id(random)
4. Identifier (id) System & Overflow RulesFixed block names are enclosed in double quotes 
("...") to prevent collisions, while unquoted keywords trigger built-in identifier generators:
CommandDescriptionid("name")User-defined static block or line label.id("")Empty / 
self-reference identifier; binds to the active block's ID or fallback target.id(number)Assigns sequential 
numeric IDs (1, 2, 3...).id(words)Assigns sequential letter IDs; rolls over to multi-letter 
combinations (SE...) once single letters are exhausted.id(random)Generates a random alphanumeric 
identifier.id(random_number)Generates a random number between 0 and 1000, expanding digit range on 
overflow.id(random_words)Generates random letter IDs, expanding to multi-character strings on overflow.

5. Loops & Intervals (loop & frange)loop(id("m"), id("a")): Loops between the two specified identifier 
points.loop(frange.(id("m")-id("a"))): Iterates step-by-step across the mathematical interval from 
id("m") to id("a") (automatically detects ascending or descending ranges).

6. Links & Labeling (href)External links, local files, or modules are declared in a single line using 
space-separated modifier attributes (show, id):Plaintexthref "link" show("display name") id("label") /href
