export default function(data, title, unit, binCount) {
    const width = 800
    const height = 700

    const svg = d3.select("#graph").append("svg")
        .attr("width", width)
        .attr("height", height)

    let times = data.sort((a, b) => a - b);

    // Remove outliers
    const q1 = d3.quantile(times, .25)
    const q3 = d3.quantile(times, .75)
    const iqr = q3 - q1
    const kkk = times.length
    times = times.filter(x => x < (q3 + 1.5 * iqr) && x > (q1 - 1.5 * iqr))

    const bins = d3.bin().thresholds(binCount)(times)

    const y = d3.scaleLinear()
        .domain([0, d3.max(bins, d => d.length)])
        .range([height - 30, height * 2 / 3 - 30])

    const x = d3.scaleLinear()
        .domain([bins[0].x0, bins[bins.length - 1].x1])
        .range([30, width - 30])

    // Graph
    svg.append("g")
        .attr("fill", "tomato")
        .selectAll()
        .data(bins)
        .join("rect")
        .attr("y", d => y(d.length))
        .attr("width", d => x(d.x1) - x(d.x0) - 2)
        .attr("x", d => x(d.x0) + 1)
        .attr("height", d => y(0) - y(d.length))

    // X axis
    svg.append("g")
        .attr("transform", `translate(0,${height - 25})`)
        .call(d3.axisBottom(x))

    // Title
    svg.append("text")
        .attr("font-size", 30)
        .attr("text-anchor", "middle")
        .attr("font-weight", "bold")
        .attr("x", width / 2)
        .attr("y", 100)
        .text(title)

    // Results

    const y0 = 180;
    const dist = 40;
    const gap = 20;
    
    svg.append("text")
        .attr("font-size", 20)
        .attr("text-anchor", "end")
        .attr("x", (width - gap) / 2)
        .attr("y", y0)
        .text("Minimum:")

    svg.append("text")
        .attr("font-size", 20)
        .attr("text-anchor", "start")
        .attr("x", (width + gap) / 2)
        .attr("y", y0)
        .text("Minimum")
        .text(d3.min(data) + unit)

    svg.append("text")
        .attr("font-size", 20)
        .attr("text-anchor", "end")
        .attr("x", (width - gap) / 2)
        .attr("y", y0 + dist)
        .text("Average:")

    svg.append("text")
        .attr("font-size", 20)
        .attr("text-anchor", "start")
        .attr("x", (width + gap) / 2)
        .attr("y", y0 + dist)
        .text((d3.mean(data) | 0) + unit)

    svg.append("text")
        .attr("font-size", 20)
        .attr("text-anchor", "end")
        .attr("x", (width - gap) / 2)
        .attr("y", y0 + 2 * dist)
        .text("Maximum:")

    svg.append("text")
        .attr("font-size", 20)
        .attr("text-anchor", "start")
        .attr("x", (width + gap) / 2)
        .attr("y", y0 + 2 * dist)
        .text(d3.max(data) + unit)

    svg.append("text")
        .attr("font-size", 20)
        .attr("text-anchor", "end")
        .attr("x", (width - gap) / 2)
        .attr("y", y0 + 3 * dist)
        .text("Std dev:")

    svg.append("text")
        .attr("font-size", 20)
        .attr("text-anchor", "start")
        .attr("x", (width + gap) / 2)
        .attr("y", y0 + 3 * dist)
        .text((d3.deviation(data) | 0) + unit)
}
