# NumeriCore API Reference

NumeriCore reads one JSON object from **stdin** and writes one JSON object to
**stdout**. There is no network transport; "API" here means the JSON contract.

## Request

```json
{
    "operation": "calculate_area",
    "start": 0,
    "end": 4,
    "x_values": [0, 1, 2, 3, 4],
    "y_values": [0, 1, 4, 9, 16],
    "midpoint_values": [0.25, 2.25, 6.25, 12.25],
    "actual_area": 21.3333333333333
}
```

| Field | Type | Required | Notes |
|---|---|---|---|
| `operation` | string | yes | `calculate_area` or `calculate_volume` |
| `start` | number | yes | start of the interval; finite |
| `end` | number | yes | end of the interval; finite, `> start` |
| `x_values` | number[] | yes | ≥ 3 points, equally spaced, first = `start`, last = `end`; count must give an **even** number of intervals |
| `y_values` | number[] | yes | same length as `x_values`, finite |
| `midpoint_values` | number[] | yes | exactly one per interval (`x_values.length − 1`), finite |
| `actual_area` | number | no | reference value when `operation = calculate_area` |
| `actual_volume` | number | no | reference value when `operation = calculate_volume` |

Provide at most one of `actual_area` / `actual_volume`. The reference value whose
key matches `operation` enables error analysis; the other key is ignored.

## Success response

```json
{
    "success": true,
    "data": {
        "step_size": 1.0,
        "actual_value": 21.3333333333333,
        "comparison_available": true,
        "results": {
            "trapezoidal": { "estimated_value": 22.0, "absolute_error": 0.666…, "percentage_error": 3.125… },
            "midpoint":    { "estimated_value": 21.0, "absolute_error": 0.333…, "percentage_error": 1.562… },
            "simpson":     { "estimated_value": 21.333…, "absolute_error": 3.19e-14, "percentage_error": 1.49e-13 }
        },
        "most_accurate_methods": ["simpson"]
    },
    "error": null
}
```

| Field | Type | Notes |
|---|---|---|
| `success` | bool | `true` on a completed calculation |
| `data.step_size` | number | `(end − start) / intervals` |
| `data.actual_value` | number \| null | the reference value, or `null` if none given |
| `data.comparison_available` | bool | `true` when a reference value enabled error analysis |
| `data.results.<method>.estimated_value` | number | the method's integral estimate |
| `data.results.<method>.absolute_error` | number \| null | `null` when no reference value |
| `data.results.<method>.percentage_error` | number \| null | `null` when no reference value, or reference is 0 |
| `data.most_accurate_methods` | string[] | method(s) with the smallest absolute error (ties reported together); empty without a reference |

## Error response

```json
{
    "success": false,
    "data": null,
    "error": { "code": "INVALID_INTERVAL_COUNT", "message": "Simpson's 1/3 Rule requires an even number of intervals." }
}
```

The process exits non-zero only for stdin-level problems (`EMPTY_REQUEST`,
`INPUT_TOO_LARGE`, `INTERNAL_ERROR`); validation and calculation errors are
returned as a normal `success:false` body with exit code 0.

### Error codes

| Stage | Codes |
|---|---|
| Input / parsing | `EMPTY_REQUEST`, `INPUT_TOO_LARGE`, `INVALID_JSON`, `INVALID_JSON_FORMAT`, `INVALID_JSON_FIELD`, `INVALID_NUMERIC_VALUE`, `MISSING_OPERATION`, `INVALID_START`, `INVALID_END`, `AMBIGUOUS_ACTUAL_VALUE`, `INVALID_ACTUAL_VALUE` |
| Validation | `INVALID_OPERATION`, `INVALID_RANGE`, `INVALID_MEASUREMENT_COUNT`, `MISMATCHED_MEASUREMENTS`, `INVALID_INTERVAL_COUNT`, `INVALID_MIDPOINT_COUNT`, `INVALID_X_VALUE`, `INVALID_Y_VALUE`, `INVALID_MIDPOINT_VALUE`, `RANGE_MISMATCH`, `UNEQUAL_SPACING` |
| Calculation | `CALCULATION_ERROR`, `INTERNAL_ERROR` |

Limits (see `include/utils/Constants.h`): request body ≤ 1 MiB, measurement
arrays ≤ 100000 entries, ≥ 3 points.
