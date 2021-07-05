# ConfusableMatcher
ConfusableMatcher provides an `IndexOf` method with capability of including string (key) to string (value) mapping as part of matching process.

Features:
 * **Matching on word boundary** - looks for `[^\p{L}\p{N}]` chars
 * **Repeated string matching** - meaning `AAA` input would match with `A` needle
 * **Skipped string matching** - meaning `ABA` input with `B` skip would match `A` needle
 * **State push limit** - similar to regex timeouts, limits internal state machine operations

See `Export.h` for C-like function exports

# Limitations

Due to limitations in Google's `dense_hash_map`, key or value cannot start with `\x00` or `\x01`. These values are reserved as empty or deleted key/value placeholders.

# Example

```
// Main string to string map
std::vector<std::pair<std::string, std::string>> map;

// Map "A" to mean "@" when searching for a match
map.push_back(std::pair("A", "@"));

// Also adds default mappings [A-Z] -> [A-Z], [A-Z] -> [a-z], [0-9] -> [0-9]
// Specify "()" as a string to skip when searching for a match
auto matcher = ConfusableMatcher(map, { "()" });

// If you have a constant "Contains" string, you can
// compute some matching processing beforehand
// to aid ConfusableMatcher to achieve more performance.
//
// Next time when searching for same needle, keep this handle.
//
// Note that this handle must match with what you pass to IndexOf
auto posPointers = matcher.ComputeStringPosPointers("AB");

CMOptions opts = { };
opts.MatchRepeating = true;
opts.MatchOnWordBoundary = true;
opts.ContainsPosPointers = posPointers;

auto res = matcher.IndexOf(
    "!()@8()!", // Input string
    "AB", // What to search for
    opts // Options
);

// res.Status = MATCH --- Got a match
// res.Start = 3 --- Index
// res.Size = 2 --- Length

matcher.FreeStringPosPointers(posPointers);
```
