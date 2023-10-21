<script>
	import { Scatter } from 'svelte-chartjs';
	import { Chart, LinearScale } from 'chart.js';
	import { PointElement } from 'chart.js';
	import { onMount } from 'svelte';

	Chart.register(PointElement);
	Chart.register(LinearScale);

	let dataFromAPI = {plotdata: []};
	let chartData = {
		datasets: [{
			label: 'データセット1',
			data: [],
			backgroundColor: 'rgba(75, 192, 192, 0.2)',
			borderColor: 'rgba(75, 192, 192, 1)',
			borderWidth: 1
		}]
	};

	let options = {};

	async function fetchData() {
		try {
			const response = await fetch("http://127.0.0.1:8000/get_data");
			if (response.ok) {
				dataFromAPI = await response.json();
				chartData.datasets[0].data = dataFromAPI.plotdata;
				console.log("Fetched Data: ", dataFromAPI);
			} else {
				console.error(`API Error: ${response.statusText}`);
			}
		} catch (error) {
			console.error(`Fetch Error: ${error}`);
		}
	}

	onMount(fetchData);
</script>

{#if dataFromAPI.plotdata.length > 0}
	<Scatter data={chartData} {options} />
{:else}
	<p>データがありません。</p>
{/if}
